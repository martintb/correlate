#include <algorithm>
#include <memory>
#include <iostream>
#include <mpi.h>
#include <boost/algorithm/string/classification.hpp> 
#include <boost/algorithm/string/split.hpp>
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

#include "Config.hpp"
#include "AtomGroup.hpp"

using namespace std;

bool stepper(Config *conf) {
  vector<string> sel1,sel2;
  boost::split(sel1,conf->type1,boost::is_any_of(", "),boost::token_compress_on);
  boost::split(sel2,conf->type2,boost::is_any_of(", "),boost::token_compress_on);

  int natoms1=-1,natoms2=-1;
  AtomGroup::ptr AG(nullptr),AG1(nullptr),AG2(nullptr); // only root will have initalized AG

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

  conf->chunk(natoms1,conf->mpi_size,natoms2,1);
  int imin = conf->ichunks->mindex;
  int imax = conf->ichunks->maxdex;
  int jmin = conf->jchunks->mindex;
  int jmax = conf->jchunks->maxdex;

  vector<float> box(3,0.0f);
  vector<float> x1Chunk,y1Chunk,z1Chunk;
  vector<float> x2Chunk,y2Chunk,z2Chunk;
  x1Chunk.resize(conf->ichunks->chunk_size);
  y1Chunk.resize(conf->ichunks->chunk_size);
  z1Chunk.resize(conf->ichunks->chunk_size);
  x2Chunk.resize(conf->jchunks->chunk_size);
  y2Chunk.resize(conf->jchunks->chunk_size);
  z2Chunk.resize(conf->jchunks->chunk_size);

  vector<float> x1,y1,z1;
  vector<float> x2,y2,z2;
  x1.resize(natoms1);
  y1.resize(natoms1);
  z1.resize(natoms1);
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

      x1 = AG1->toSTLVec(0);
      y1 = AG1->toSTLVec(1);
      z1 = AG1->toSTLVec(2);
      x2 = AG2->toSTLVec(0);
      y2 = AG2->toSTLVec(1);
      z2 = AG2->toSTLVec(2);

      box[0] = AG1->lx;
      box[1] = AG1->ly;
      box[2] = AG1->lz;

      cout << "--> Distributing positions to processes..." << endl;
    }
    MPI::COMM_WORLD.Bcast(&(box.front()),box.size(),MPI::FLOAT,0);
    MPI::COMM_WORLD.Bcast(&(x1.front()),x1.size(),MPI::FLOAT,0);
    MPI::COMM_WORLD.Bcast(&(y1.front()),y1.size(),MPI::FLOAT,0);
    MPI::COMM_WORLD.Bcast(&(z1.front()),z1.size(),MPI::FLOAT,0);
    MPI::COMM_WORLD.Bcast(&(x2.front()),x2.size(),MPI::FLOAT,0);
    MPI::COMM_WORLD.Bcast(&(y2.front()),y2.size(),MPI::FLOAT,0);
    MPI::COMM_WORLD.Bcast(&(z2.front()),z2.size(),MPI::FLOAT,0);

    if (conf->isRoot())
      cout << "--> Gathering data chunks..." << endl;
    copy(x1.begin()+imin,x1.begin()+imax,x1Chunk.begin());
    copy(y1.begin()+imin,y1.begin()+imax,y1Chunk.begin());
    copy(z1.begin()+imin,z1.begin()+imax,z1Chunk.begin());
    copy(x2.begin()+jmin,x2.begin()+jmax,x2Chunk.begin());
    copy(y2.begin()+jmin,y2.begin()+jmax,y2Chunk.begin());
    copy(z2.begin()+jmin,z2.begin()+jmax,z2Chunk.begin());
    MPI::COMM_WORLD.Barrier();

    for (int i=0; i<conf->mpi_size;i++) {
      MPI::COMM_WORLD.Barrier();
      if (i==conf->mpi_rank) {
        cout << i << " ";
        cout << box[0] << " ";
        cout << box[1] << " ";
        cout << box[2] << endl;
        cout << x1[0] << " ";
        cout << y1[0] << " ";
        cout << z1[0] << " ";
        cout << x2[0] << " ";
        cout << y2[0] << " ";
        cout << z2[0] << endl;
        cout << x1[imax-1] << " ";
        cout << y1[imax-1] << " ";
        cout << z1[imax-1] << " ";
        cout << x2[jmax-1] << " ";
        cout << y2[jmax-1] << " ";
        cout << z2[jmax-1] << endl;
        cout << x1Chunk.back()<< " ";
        cout << y1Chunk.back()<< " ";
        cout << z1Chunk.back()<< " ";
        cout << x2Chunk.back()<< " ";
        cout << y2Chunk.back()<< " ";
        cout << z2Chunk.back()<< endl;
        cout << "----------------------------" << endl;
      }
    }
  }



  return true;
}
