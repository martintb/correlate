#include <mpi.h>
#include <iostream> //cout, endl
#include <string>
#include <vector>
#include <sstream> // ostringstream

#include "debug.hpp"
#include "Timer.hpp"
#include "Config.hpp"
#include "Chunker.hpp"
#include "AtomGroup.hpp"
#include "Writer.hpp"
#include "kernel_switcher.hpp"

using namespace std;

void stepper(Config::ptr &conf) {

  Timer timer;
  timer.tic("total");

  //############################//
  //### INITIAL FILE READING ###//
  //############################//
  AtomGroup::ptr AG,AG1,AG2; // only root will have initalized AG,AG1,AG2
  vector<int> mpi_intbuf(6,-1);
  timer.tic("init");
  conf->printHeader("FILE INFO");
  if (conf->isRoot()) {
    AG = AtomGroup::make(conf->topo_file->path.string(),conf->trj_file->path.string());

    AG1 = AG->select_types(conf->type1);
    AG2 = AG->select_types(conf->type2);

    // Handle negative frame start/stop
    int frame_end = conf->frame_end; //frame_end
    if (frame_end == -1) {
      frame_end = AG->numFrames;
    }

    int frame_start = conf->frame_start; //frame_start
    if (frame_start<0) {
      if ((frame_end+frame_start)<0){
        cerr << "==> Not enough frames for end-referenced frame_start: ";
        cerr << conf->frame_start << endl;
        cerr << "==> Setting frame_start to 0!";
        frame_start = 0;
      } else {
        frame_start = frame_end + frame_start;
      }
    }
    int output_freq = conf->output_freq; 
    if (output_freq==-1) {
      output_freq = (frame_end-frame_start)/conf->frame_step;
    }

    conf->natoms1 = AG1->natoms;
    conf->natoms2 = AG2->natoms;
    conf->frame_start = frame_start;
    conf->frame_end = frame_end;
    conf->output_freq = output_freq;
  }
  MPI::COMM_WORLD.Barrier();
  conf->sync();

  conf->printHeader("CONFIGURATION");
  conf->print(0);

  if (conf->natoms1<=0 or conf->natoms2<=0) {
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
  Chunker Chunker1(conf->natoms1,conf->mpi_size);
  Chunker Chunker2(conf->natoms2,1);
  conf->printHeader("CHUNK1 INFO");
  Chunker1.print();
  conf->printHeader("CHUNK2 INFO");
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
  vector<int> masterMol1,masterMol2;
  auto writer = Writer::get(conf);

  //##################//
  //### FRAME LOOP ###//
  //##################//
  conf->printHeader("FRAME LOOP");
  ostringstream oss;
  oss << "> Starting frame loop with (start/stop/step): ";
  oss << "(" << conf->frame_start << "/" << conf->frame_end << "/" << conf->frame_step << ")";
  conf->print(oss.str());
  timer.toc("init",/*printSplit=*/true);
  for (int frame=conf->frame_start;frame<conf->frame_end;frame+=conf->frame_step) 
  {
    timer.tic("frame loop");
    timer.tic("atomgroup io");
    if (conf->isRoot()) {
      cout << "--> Reading frame " << frame << endl;
      AG->readFrame(frame);
      cout << "--> Successfully read frame " << frame << endl;

      AG->getBox(box);
      cout << "--> Box (lx,ly,lz): ";
      cout << box[0] << ", " << box[1] << ", " << box[2] << endl;

      AG1->getPositions(masterX1,masterY1,masterZ1);
      AG2->getPositions(masterX2,masterY2,masterZ2);
      AG1->getMolecules(masterMol1);
      AG2->getMolecules(masterMol2);
    }
    MPI::COMM_WORLD.Barrier();
    timer.toc("atomgroup io",/*printSplit=*/true);

    conf->print("--> Distributing atomdata to mpi-processes...");
    timer.tic("comm");
    MPI::COMM_WORLD.Bcast(&box.front(),box.size(),MPI::FLOAT,0);
    Chunker1.distribute( &masterX1, &x1);
    Chunker1.distribute( &masterY1, &y1);
    Chunker1.distribute( &masterZ1, &z1);
    Chunker1.distribute( &masterMol1, &mol1);
    Chunker2.distribute( &masterX2, &x2);
    Chunker2.distribute( &masterY2, &y2);
    Chunker2.distribute( &masterZ2, &z2);
    Chunker2.distribute( &masterMol2, &mol2);
    timer.toc("comm",/*printSplit=*/true);
    conf->print("--> Done distributing.");

    timer.tic("kernel");
    int offset = Chunker1.mindex_list[conf->mpi_rank];
    kernel_switcher( 
                     frame,
                     conf,
                     writer,
                     x1, y1, z1,
                     x2, y2, z2,
                     mol1, mol2,
                     box, 
                     offset
                    );
    timer.toc("kernel",/*printSplit=*/true);


    MPI::COMM_WORLD.Barrier();
    timer.toc("frame loop",/*printSplit=*/true);
    conf->print("-----------------------------------------------------------");
  }
  conf->print("> Done! Frame loop finished successfully!");

  conf->printHeader("ALL DONE");

  conf->print(" ");
  conf->print(">>> CONFIGURATION");
  conf->print(0);

  if (conf->isRoot()) {
    cout << "\n";
    cout << ">>> PROC TIMING STATS (SECONDS)";
    cout << endl;
  }
  timer.toc("total");

  MPI::COMM_WORLD.Barrier();
  timer.print_stats();
}
