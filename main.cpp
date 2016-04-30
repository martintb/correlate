#include <string>
#include <mpi.h>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "Config.hpp"
#include "parse_opts.hpp"

using namespace std;

void stepper(Config *conf); // forward declaration rather than mini hpp

int main(int argc, char* argv[]) 
{
  MPI::Init();

  Config conf; // each proc carries a "configuration" object

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
  } else {
    conf.sync(); // sync int and float values in conf
  }
  // conf.print(); // print configuration info of each proc

  stepper(&conf);

  MPI::Finalize(); // must be called by all procs before exiting
  return EXIT_SUCCESS;
}

