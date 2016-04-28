#include <algorithm>
#include <memory>
#include <iostream>
#include <mpi.h>
#include <boost/algorithm/string/classification.hpp> 
#include <boost/algorithm/string/split.hpp>
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

#include "Config.hpp"
#include "Chunker.hpp"
#include "AtomGroup.hpp"

using namespace std;

bool stepper(Config *conf) {
  vector<string> sel1,sel2;
  boost::split(sel1,conf->type1,boost::is_any_of(", "),boost::token_compress_on);
  boost::split(sel2,conf->type2,boost::is_any_of(", "),boost::token_compress_on);

  int natoms1=-1,natoms2=-1;
  AtomGroup::ptr AG,AG1,AG2; // only root will have initalized AG

  int frame_start,frame_end,frame_step;
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

    AG1 = AG->select(sel1);
    AG2 = AG->select(sel2);

    natoms1 = AG1->natoms;
    natoms2 = AG2->natoms;

    frame_step = conf->frame_step;
    frame_start = conf->frame_start;
    frame_end = conf->frame_end;
    if (frame_end == -1) 
      frame_end = AG->dptr->numFrames;

  }

  MPI::COMM_WORLD.Bcast(&frame_start,1,MPI::INT,0);
  MPI::COMM_WORLD.Bcast(&frame_end,1,MPI::INT,0);
  MPI::COMM_WORLD.Bcast(&frame_step,1,MPI::INT,0);
  MPI::COMM_WORLD.Bcast(&natoms1,1,MPI::INT,0);
  MPI::COMM_WORLD.Bcast(&natoms2,1,MPI::INT,0);

  Chunker Chunker1(natoms1,conf->mpi_size);
  Chunker Chunker2(natoms2,1);

  if (conf->isRoot())  {
    Chunker1.print();
    cout << "===================================" << endl;
    Chunker2.print();
    cout << "===================================" << endl;
  }

  vector<float> box(3,0.0f);
  vector<float> masterX1,masterY1,masterZ1;
  vector<float> masterX2,masterY2,masterZ2;
  vector<float> x1,y1,z1;
  vector<float> x2,y2,z2;
  x2.resize(natoms2);
  y2.resize(natoms2);
  z2.resize(natoms2);

  MPI::COMM_WORLD.Barrier();
  if (conf->isRoot())
    cout << "Starting frame loop..." << endl;
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
      x2 = masterX2;
      y2 = masterY2;
      z2 = masterZ2;

      box[0] = AG1->lx;
      box[1] = AG1->ly;
      box[2] = AG1->lz;

      cout << "--> Distributing positions to processes..." << endl;
    }
    MPI::COMM_WORLD.Bcast(&box.front(),box.size(),MPI::FLOAT,0);
    Chunker1.distribute( &masterX1, &x1);
    Chunker1.distribute( &masterY1, &y1);
    Chunker1.distribute( &masterZ1, &z1);


    MPI::COMM_WORLD.Bcast(&x2.front(),natoms2,MPI::FLOAT,0);
    MPI::COMM_WORLD.Bcast(&y2.front(),natoms2,MPI::FLOAT,0);
    MPI::COMM_WORLD.Bcast(&z2.front(),natoms2,MPI::FLOAT,0);
    // Chunker2.distribute( &masterX2, &x2);
    // Chunker2.distribute( &masterY2, &y2);
    // Chunker2.distribute( &masterZ2, &z2);
    MPI::COMM_WORLD.Barrier();
    if (conf->isRoot()) 
      cout << "--> Done distributing." << endl;

    for (int i=0; i<conf->mpi_size;i++) {
      MPI::COMM_WORLD.Barrier();
      if (i==conf->mpi_rank) {
        cout << i << " ";
        cout << box[0] << " ";
        cout << box[1] << " ";
        cout << box[2] << " ";
        cout << "||" << " ";
        cout << x1.back()<< " ";
        cout << y1.back()<< " ";
        cout << z1.back()<< " ";
        cout << "||" << " ";
        cout << x2.back()<< " ";
        cout << y2.back()<< " ";
        cout << z2.back()<< endl;
        cout << "----------------------------" << endl;
      }
    }
  }


  if (AG1)
    AG1.reset();

  if (AG2)
    AG2.reset();

  if (AG)
    AG.reset();

  return true;
}
