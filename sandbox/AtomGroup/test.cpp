#include <string>
#include <mpi.h>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "AtomGroup.hpp"
#include "Config.hpp"
#include "parse_opts.hpp"

using namespace std;

void stepper(Config *conf); // forward declaration rather than mini hpp

int main(int argc, char* argv[]) 
{
  MPI::Init();

  Config conf; // each proc carries a "configuration" object

  conf.print("============ PARSE INPUT ============");
  bool success=false; //assume failure
  if (conf.isRoot()) {
    bool success1=false;
    bool success2=false;
    success1 = parse_opts(argc,argv,&conf);
    if (success1)
      success2 = conf.setKernelFromStr();
    success = (success1 and success2);
  }

  // All MPI procs must fail together so we transmit
  // the parse_opts result and react together
  MPI::COMM_WORLD.Bcast(&success,1,MPI::BOOL,0);
  if (not success) {
    MPI::Finalize(); // must be called by all procs before exiting
    return EXIT_FAILURE;
  } 

  int frame = 380;
  AtomGroup::ptr AG = AtomGroup::make(conf.topoPath,conf.trjPath);
  AG->readFrame(frame);
  cout << "--> Successfully read frame " << frame << endl;

  vector<float> x1,y1,z1;
  vector<float> box(3,-1.0f);
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


  MPI::Finalize(); // must be called by all procs before exiting
  return EXIT_SUCCESS;
}
