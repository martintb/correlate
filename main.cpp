#include <string>
#include <mpi.h>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "Config.hpp"
#include "parse_opts.hpp"

#include <armadillo>

using namespace std;

void stepper(Config *conf);

int main(int argc, char* argv[]) 
{
  MPI::Init();

  // int mpi_size = MPI::COMM_WORLD.Get_size();
  // int mpi_rank = MPI::COMM_WORLD.Get_rank();

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
    conf.sync();
  }

  // MPI::COMM_WORLD.Bcast(&(conf.frame_start),1,MPI::INT,0);
  // MPI::COMM_WORLD.Bcast(&(conf.frame_end),  1,MPI::INT,0);
  // MPI::COMM_WORLD.Bcast(&(conf.frame_step), 1,MPI::INT,0);
  // MPI::COMM_WORLD.Bcast(&(conf.nthreads),   1,MPI::INT,0);
  // MPI::COMM_WORLD.Bcast(&(conf.kernel),     1,MPI::INT,0);
  // MPI::COMM_WORLD.Bcast(&(conf.dr),         1,MPI::FLOAT,0);
  // MPI::COMM_WORLD.Bcast(&(conf.rmax),       1,MPI::FLOAT,0);

  conf.print("===========================");
  for (int i=0;i<conf.mpi_size;++i) {
    if (i==conf.mpi_rank) {
      conf.print();
      cout << "===========================" << endl;
    }
    MPI::COMM_WORLD.Barrier();
  }

  stepper(&conf);


  MPI::Finalize(); // must be called by all procs before exiting
  return EXIT_SUCCESS;
}

