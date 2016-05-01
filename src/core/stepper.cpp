#include <iostream> //cout, endl
#include <fstream> //ofstream
#include <iomanip> //setw
#include <algorithm>
#include <memory>
#include <mpi.h>
#include <string>
#include <vector>
#include <sstream> // ostringstream

#include <boost/algorithm/string/classification.hpp> 
#include <boost/algorithm/string/split.hpp>
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

#include "Timer.hpp"
#include "Config.hpp"
#include "Chunker.hpp"
#include "AtomGroup.hpp"
#include "kernels.hpp"

using namespace std;

void stepper(Config *conf) {

  Timer timer;
  timer.tic("stepper");

  //############################//
  //### INITIAL FILE READING ###//
  //############################//
  vector<string> sel1,sel2;
  AtomGroup::ptr AG,AG1,AG2; // only root will have initalized AG,AG1,AG2
  vector<int> mpi_intbuf(6,-1);
  timer.tic("io");
  if (conf->isRoot()) {
    conf->buildPaths();
    AG = make_shared<AtomGroup>(conf->xmlPath,conf->dcdPath);
    AG->readFiles();
    conf->print("============= XML INFO =============");
    AG->xptr->contains();
    AG->xptr->printFileInfo();
    conf->print("============= DCD INFO =============");
    AG->dptr->contains();
    AG->dptr->printFileInfo();

    boost::split(sel1,conf->type1,boost::is_any_of(", "),boost::token_compress_on);
    boost::split(sel2,conf->type2,boost::is_any_of(", "),boost::token_compress_on);

    bool selfHist;
    if (conf->kernel == Config::inter_mol_rdf) {
      selfHist = false;
    } else {
      selfHist = conf->type1.compare(conf->type2)==0;
    }
    int natoms1 = AG->check_select_size(sel1); //natoms1
    int natoms2 = AG->check_select_size(sel2); //natoms2
    int frame_start = conf->frame_start; //frame_start
    int frame_step = conf->frame_step; //frame_step
    int frame_end = conf->frame_end; //frame_end
    if (frame_end == -1) 
      frame_end = AG->dptr->numFrames;

    mpi_intbuf[0] = selfHist;
    mpi_intbuf[1] = natoms1;
    mpi_intbuf[2] = natoms2;
    mpi_intbuf[3] = frame_start;
    mpi_intbuf[4] = frame_step;
    mpi_intbuf[5] = frame_end;
  }
  MPI::COMM_WORLD.Barrier();
  timer.toc("io",/*printSplit=*/true);

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

  //###############################//
  //### INTIALIZE DATA CHUNKERS ###//
  //###############################//
  Chunker Chunker1(natoms1,conf->mpi_size);
  Chunker Chunker2(natoms2,1);
  conf->print("============= CHUNK1 INFO =============");
  Chunker1.print();
  conf->print("============= CHUNK2 INFO =============");
  Chunker2.print();

  //#######################//
  //### FRAME LOOP PREP ###//
  //#######################//
  vector<float> box(3,0.0f);
  vector<float> masterX1,masterY1,masterZ1;
  vector<float> masterX2,masterY2,masterZ2;
  vector<float> x1,y1,z1;
  vector<float> x2,y2,z2;
  vector<int> mol1,mol2;
  vector<float> mol1f,mol2f;
  vector<float> masterMol1f,masterMol2f;
  unsigned long pair_count=0;

  conf->buildSpace();
  vector<unsigned long> procVecInt;
  vector<float> procVecFloat;
  if (conf->kernel == Config::histogram or 
      conf->kernel == Config::rdf or
      conf->kernel == Config::inter_mol_rdf
     ) 
  {
    procVecInt.assign(conf->xsize,0);
  } else if (conf->kernel == Config::omega) {
    procVecFloat.assign(conf->xsize,0.0f);
  }

  //##################//
  //### FRAME LOOP ###//
  //##################//
  conf->print("============= FRAME LOOP =============");
  ostringstream oss;
  oss << "> Starting frame loop with (start/stop/step): ";
  oss << "(" << frame_start << "/" << frame_end << "/" << frame_step << ")";
  conf->print(oss.str());
  int num_frames = 0;
  for (int frame=frame_start;frame<frame_end;frame+=frame_step) 
  {
    timer.tic("frame_loop");
    timer.tic("io");
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

      masterMol1f = AG1->STLMolFloat();
      masterMol2f = AG2->STLMolFloat();

      box[0] = AG1->lx;
      box[1] = AG1->ly;
      box[2] = AG1->lz;

      cout << "--> Box (lx,ly,lz): ";
      cout << box[0] << ", ";
      cout << box[1] << ", ";
      cout << box[2] << endl;
    }
    MPI::COMM_WORLD.Barrier();
    timer.toc("io",/*printSplit=*/true);

    conf->print("--> Distributing atomdata to mpi-processes...");
    timer.tic("comm_time");
    MPI::COMM_WORLD.Bcast(&box.front(),box.size(),MPI::FLOAT,0);
    Chunker1.distribute( &masterX1, &x1);
    Chunker1.distribute( &masterY1, &y1);
    Chunker1.distribute( &masterZ1, &z1);
    Chunker2.distribute( &masterX2, &x2);
    Chunker2.distribute( &masterY2, &y2);
    Chunker2.distribute( &masterZ2, &z2);
    // distribute deals in floats, but we need the 
    // mol numbers as ints for equality comparsons
    Chunker1.distribute( &masterMol1f, &mol1f);
    Chunker2.distribute( &masterMol2f, &mol2f);
    mol1.assign(mol1f.begin(),mol1f.end());
    mol2.assign(mol2f.begin(),mol2f.end());
    timer.toc("comm_time",/*printSplit=*/true);
    conf->print("--> Done distributing.");

    //####################//
    //### printProcXYZ ###//
    //####################//
    if (conf->kernel == Config::printProcXYZ) {
      conf->print("--> Calling kernel: printProcXYZ");
      printProcXYZ(frame,"xyz1",x1,y1,z1);
      printProcXYZ(frame,"xyz2",x2,y2,z2);
      
    //#####################//
    //### histogram/rdf ###//
    //#####################//
    } else if (conf->kernel == Config::histogram or conf->kernel == Config::rdf) {
      conf->print("--> Calling kernel: histogram/rdf");
      int offset = Chunker1.mindex_list[conf->mpi_rank];
      timer.tic("histogram_kernel");
      histogram(procVecInt,
                x1,y1,z1,
                x2,y2,z2,
                box,
                selfHist,
                conf->xmax,conf->dx,
                offset);
      timer.toc("histogram_kernel",/*printSplit=*/true);

    //###########################//
    //### inter_histogram/rdf ###//
    //###########################//
    } else if (conf->kernel == Config::inter_mol_rdf) {
      conf->print("--> Calling kernel: inter_mol_rdf");
      timer.tic("ihist_kernel");
      pair_count=0;
      inter_mol_histogram(procVecInt,
                          x1,y1,z1,
                          x2,y2,z2,
                          mol1,mol2,
                          box, conf->xmax,conf->dx,pair_count);
      timer.toc("ihist_kernel",/*printSplit=*/true);

    //#############//
    //### omega ###//
    //#############//
    } else if (conf->kernel == Config::omega) {
      conf->print("--> Calling kernel: omega");
      int offset = Chunker1.mindex_list[conf->mpi_rank];
      timer.tic("omega_kernel");
      omega(procVecFloat,
            x1,y1,z1,
            x2,y2,z2,
            box,
            selfHist,
            conf->xmax,conf->dx,
            offset);
      timer.toc("omega_kernel",/*printSplit=*/true);

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


    timer.toc("frame_loop",/*printSplit=*/true);
    num_frames++;
    conf->print("-----------------------------------------------------------");
  }
  conf->print("> Done! Frame loop finished successfully!");

  conf->print("============= POSTPROCESSING =============");
  //##############################//
  //### GATHER FRAME LOOP DATA ###//
  //##############################//
  vector<unsigned long> allVecInt(conf->xsize,0);
  vector<float> allVecFloat(conf->xsize,0.0f);
  vector<float> outVec;
  conf->print("--> Gathering data from each proc...");
  if ( 
       conf->kernel == Config::histogram or 
       conf->kernel == Config::rdf or
       conf->kernel == Config::inter_mol_rdf
     ) 
  {
    MPI::COMM_WORLD.Reduce(&procVecInt.front(),&allVecInt.front(),procVecInt.size(),
                         MPI::UNSIGNED_LONG,MPI::SUM,0);
    outVec.assign(allVecInt.begin(),allVecInt.end());
  } else if (conf->kernel == Config::omega) {
    MPI::COMM_WORLD.Reduce(&procVecFloat.front(),&allVecFloat.front(),procVecFloat.size(),
                         MPI::FLOAT,MPI::SUM,0);
    outVec.assign(allVecFloat.begin(),allVecFloat.end());
  }

  unsigned long all_pair_count = 0;
  if ( conf->kernel == Config::inter_mol_rdf) 
  {
    MPI::COMM_WORLD.Reduce(&pair_count,&all_pair_count,1, MPI::UNSIGNED_LONG,MPI::SUM,0);
  }

  //#############################//
  //### SCALE AND OUTPUT DATA ###//
  //#############################//
  timer.tic("io");
  if (conf->isRoot()) {
    conf->print("--> Scaling and writing data to disk..");
    float pair_rho;
    float natoms = natoms1+natoms2;
    float box_volume = box[0]*box[1]*box[2];

    if (conf->kernel == Config::inter_mol_rdf) {
      pair_rho = all_pair_count/box_volume;
    } else if (selfHist) {
      pair_rho = natoms1*(natoms2-1)/box_volume;
    } else {
      pair_rho = natoms1*natoms2/box_volume;
    }

    float constant = 0;
    if (selfHist) {
      constant = 1.0;
    }

    int width=15;
    ofstream file(conf->outfile);
    file << "#";
    file << setw(width-1)<< "x";
    file << setw(width)  << "hist";
    file << setw(width)  << "conc";
    file << setw(width)  << "rdf";
    file << setw(width)  << "omega";
    file << endl;
    for (int i=0;i<conf->xsize;i++) {
      float x1 = i*conf->dx;
      float x2 = (i+1)*conf->dx;
      float vol = (4.0/3.0) * (3.14159) * (x2*x2*x2  - x1*x1*x1);

      file << setw(width) << x1;
      file << setw(width) << outVec[i]/num_frames; 
      file << setw(width) << outVec[i]/(num_frames*vol);
      file << setw(width) << outVec[i]/(num_frames*pair_rho*vol);
      file << setw(width) << outVec[i]/(num_frames*natoms*x2)+constant; 
      file << endl;
    }
    file.close();
  }
  MPI::COMM_WORLD.Barrier();
  timer.toc("io",/*printSplit=*/true);

  if (AG1) AG1.reset();
  if (AG2) AG2.reset();
  if (AG)  AG.reset();


  conf->print("============= ALL DONE =============");
  MPI::COMM_WORLD.Barrier();

  if (conf->isRoot()) {
    cout << "\n\n";
    cout << setw(15*4+2);
    cout << "===================== TIMINGS =====================";
    cout << endl;
  }
  timer.toc("stepper");
  timer.print_stats();
}
