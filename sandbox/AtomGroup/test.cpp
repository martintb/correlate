#include <string>
#include <sstream>
#include <iostream>
#include <mpi.h>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "AtomGroup.hpp"
#include "Config.hpp"
#include "parse_opts.hpp"
#include "NotProvidedException.hpp"

using namespace std;

void frameLoop(AtomGroup::ptr,Config::ptr,int,int,int);

int main(int argc, char* argv[]) 
{
  MPI::Init();

  Config::ptr conf = make_shared<Config>(); // each proc carries a "configuration" object

  conf->print("============ PARSE INPUT ============");
  bool success=false; //assume failure
  if (conf->isRoot()) {
    bool success1=false;
    bool success2=false;
    success1 = parse_opts(argc,argv,conf);
    if (success1)
      success2 = conf->setKernelFromStr();
    success = (success1 and success2);
  }

  // All MPI procs must fail together so we transmit
  // the parse_opts result and react together
  MPI::COMM_WORLD.Bcast(&success,1,MPI::BOOL,0);
  if (not success) {
    MPI::Finalize(); // must be called by all procs before exiting
    return EXIT_FAILURE;
  } 

  auto AG = AtomGroup::make(conf->topo_file->path.string(),conf->trj_file->path.string());

  // Handle negative frame start/stop
  int frame_end = conf->frame_end; //frame_end
  int frame_start = conf->frame_start; //frame_start
  int frame_step = conf->frame_step; //frame_start
  if (frame_end == -1) {
    frame_end = AG->numFrames;
  }

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

  frameLoop(AG,conf,frame_start,frame_end,frame_step);

  MPI::Finalize(); // must be called by all procs before exiting
  return EXIT_SUCCESS;
}

void frameLoop(AtomGroup::ptr AG,Config::ptr conf,int frame_start,int frame_end,int frame_step) {
  vector<float> x1,y1,z1;
  vector<float> vx1,vy1,vz1;
  vector<float> box(3,-1.0f);
  vector<string> tt;
  vector<int> mm;

  conf->printHeader("FRAME LOOP");
  ostringstream oss;
  oss << "> Starting frame loop with (start/stop/step): ";
  oss << "(" << frame_start << "/" << frame_end << "/" << frame_step << ")";
  conf->print(oss.str());

  for (int frame=frame_start;frame<frame_end;frame+=frame_step) {
    AG->readFrame(frame);
    cout << "--> Successfully read frame " << frame << endl;

    AG->getBox(box);
    cout << "--> Box (lx,ly,lz): ";
    cout << box[0] << ", " << box[1] << ", " << box[2] << endl;

    AG->getPositions(x1,y1,z1);
    cout << "x1Size: " << x1.size() << endl;
    cout << "y1Size: " << y1.size() << endl;
    cout << "z1Size: " << z1.size() << endl;
    cout << "x1[0] = " << x1[0] << endl;
    cout << "y1[0] = " << y1[0] << endl;
    cout << "z1[0] = " << z1[0] << endl;
    cout << "x1[end] = " <<x1[x1.size()-1] << endl;
    cout << "y1[end] = " <<y1[y1.size()-1] << endl;
    cout << "z1[end] = " <<z1[z1.size()-1] << endl;

    // AG->getTypes(tt);
    // cout << "Type Size: " << tt.size() << endl;
    // cout << "tt[0] = " << tt[0] << endl;
    // cout << "tt[10] = " << tt[10] << endl;
    // cout << "tt[100] = " << tt[100] << endl;

    AG->getMolecules(mm);
    cout << "Molecule Size: " << mm.size() << endl;
    cout << "mm[0] = " << mm[0] << endl;
    cout << "mm[10] = " << mm[10] << endl;
    cout << "mm[100] = " << mm[100] << endl;
    cout << "----------------------------------------" << endl;
  }

}

