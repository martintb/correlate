#include <iostream> //cout, endl
#include <fstream> //ofstream
#include <iomanip> //setw
#include <algorithm>
#include <memory>
#include <mpi.h>
#include <string>
#include <vector>

#include <boost/algorithm/string/classification.hpp> 
#include <boost/algorithm/string/split.hpp>
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

#include "Config.hpp"
#include "Chunker.hpp"
#include "AtomGroup.hpp"
#include "kernels.hpp"

using namespace std;

void stepper(Config *conf) {

  vector<string> sel1,sel2;
  AtomGroup::ptr AG,AG1,AG2; // only root will have initalized AG,AG1,AG2
  vector<int> mpi_intbuf;
  mpi_intbuf.reserve(6);
  if (conf->isRoot()) {
    conf->buildPaths();
    AG = make_shared<AtomGroup>(conf->xmlPath,conf->dcdPath);
    AG->readFiles();
    cout << "===================================" << endl;
    AG->xptr->contains();
    AG->xptr->printFileInfo();
    cout << "===================================" << endl;
    AG->dptr->contains();
    AG->dptr->printFileInfo();
    cout << "===================================" << endl;

    boost::split(sel1,conf->type1,boost::is_any_of(", "),boost::token_compress_on);
    boost::split(sel2,conf->type2,boost::is_any_of(", "),boost::token_compress_on);

    bool selfHist = conf->type1.compare(conf->type2)==0;//selfHist
    int natoms1 = AG->check_select_size(sel1); //natoms1
    int natoms2 = AG->check_select_size(sel2); //natoms2
    int frame_start = conf->frame_start; //frame_start
    int frame_step = conf->frame_step; //frame_step
    int frame_end = conf->frame_end; //frame_end
    if (frame_end == -1) 
      frame_end = AG->dptr->numFrames;

    mpi_intbuf.push_back(selfHist);
    mpi_intbuf.push_back(natoms1);
    mpi_intbuf.push_back(natoms2);
    mpi_intbuf.push_back(frame_start);
    mpi_intbuf.push_back(frame_step);
    mpi_intbuf.push_back(frame_end);
  }

  MPI::COMM_WORLD.Bcast(&mpi_intbuf.front(),mpi_intbuf.size(),MPI::INT,0);
  bool selfHist = static_cast<bool>(mpi_intbuf[0]);
  int natoms1 = mpi_intbuf[1];
  int natoms2 = mpi_intbuf[2];
  int frame_start = mpi_intbuf[3];
  int frame_step = mpi_intbuf[4];
  int frame_end = mpi_intbuf[5];

  if (natoms1<=0 or natoms2<=0) {
    if (conf->isRoot()) {
      cerr << "==> Error! natoms1 or natoms2 is zero." << endl;
      LOC();
    }
    MPI::Finalize();
    exit(EXIT_FAILURE);
  }

  Chunker Chunker1(natoms1,conf->mpi_size);
  Chunker Chunker2(natoms2,1);
  conf->print("Chunking for atom type 1:");
  Chunker1.print();
  conf->print("Chunking for atom type 2:");
  Chunker2.print();

  vector<float> box(3,0.0f);
  vector<float> masterX1,masterY1,masterZ1;
  vector<float> masterX2,masterY2,masterZ2;
  vector<float> x1,y1,z1;
  vector<float> x2,y2,z2;

  conf->buildSpace();
  vector<unsigned long> hist(conf->rsize,0);

  conf->print("Starting frame loop...");
  MPI::COMM_WORLD.Barrier();
  for (int frame=frame_start;frame<frame_end;frame+=frame_step) 
  {
    if (conf->isRoot()) {
      cout << "--> Reading frame " << frame << endl;
      AG->readFrame(frame);
      AG1 = AG->select(sel1);
      AG2 = AG->select(sel2);
      cout << "--> Successfully read frame " << frame << endl;

      masterX1 = AG1->toSTLVec(0);
      masterY1 = AG1->toSTLVec(1);
      masterZ1 = AG1->toSTLVec(2);
      masterX2 = AG2->toSTLVec(0);
      masterY2 = AG2->toSTLVec(1);
      masterZ2 = AG2->toSTLVec(2);

      box[0] = AG1->lx;
      box[1] = AG1->ly;
      box[2] = AG1->lz;

      cout << "--> Box (lx,ly,lz): ";
      cout << box[0] << ", ";
      cout << box[1] << ", ";
      cout << box[2] << endl;

    }
    conf->print("--> Distributing positions to processes...");
    MPI::COMM_WORLD.Bcast(&box.front(),box.size(),MPI::FLOAT,0);
    Chunker1.distribute( &masterX1, &x1);
    Chunker1.distribute( &masterY1, &y1);
    Chunker1.distribute( &masterZ1, &z1);
    Chunker2.distribute( &masterX2, &x2);
    Chunker2.distribute( &masterY2, &y2);
    Chunker2.distribute( &masterZ2, &z2);
    conf->print("--> Done distributing.");
    MPI::COMM_WORLD.Barrier();

    //####################//
    //### printProcXYZ ###//
    //####################//
    if (conf->kernel == Config::printProcXYZ) {
      conf->print("--> Calling kernel: printProcXYZ");
      printProcXYZ(frame,"xyz1",x1,y1,z1);
      printProcXYZ(frame,"xyz2",x2,y2,z2);
      
    //#################//
    //### histogram ###//
    //#################//
    } else if (conf->kernel == Config::histogram) {
      conf->print("--> Calling kernel: histogram");
      histogram(hist,
                x1,y1,z1,
                x2,y2,z2,
                box,
                selfHist,
                conf->rmax,conf->dr);

    //##############//
    //### ERROR! ###//
    //##############//
    } else {
      if (conf->isRoot()) {
        cerr << "==> Error! Stepper() is not set up for this kernel." << endl;
        LOC();
      }
      MPI::Finalize();
      exit(EXIT_FAILURE);
    }


  }

  vector<unsigned long> final_hist;
  if (conf->isRoot())  {
    cout << "--> Gathering data from each proc and writing saving to disk..." << endl;
    final_hist.assign(conf->rsize,0);
  }

  MPI::COMM_WORLD.Reduce(&hist.front(),&final_hist.front(),hist.size(),
                         MPI::UNSIGNED_LONG,MPI::SUM,0);

  if (conf->isRoot()) {
    cout << "Done! Frame loop finished successfully." << endl;
    ofstream file("hist.dat");
    for (auto i: final_hist) {
      file << i << endl;
    }
    file.close();
  }

  if (AG1) AG1.reset();
  if (AG2) AG2.reset();
  if (AG)  AG.reset();

}
