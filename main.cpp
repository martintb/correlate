#include <string>
#include <mpi.h>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "Config.hpp"
#include "parse_opts.hpp"

using namespace std;

bool stepper(Config *conf);

int main(int argc, char* argv[]) 
{
  MPI::Init();

  int mpi_size = MPI::COMM_WORLD.Get_size();
  int mpi_rank = MPI::COMM_WORLD.Get_rank();

  Config conf; // each proc carries a "configuration" object
  conf.mpi_rank = mpi_rank;
  conf.mpi_size = mpi_size;

  bool success=false; //assume failure
  if (conf.isRoot()) {
    success = parse_opts(argc,argv,&conf);
  }
  // MPI::COMM_WORLD.Barrier(); // Bcast should be blocking so this is uneccesary

  // All MPI procs must fail together so we transmit
  // the parse_opts result and react together
  MPI::COMM_WORLD.Bcast(&success,1,MPI::BOOL,0);
  if (not success) {
    MPI::Finalize(); // must be called by all procs before exiting
    return EXIT_FAILURE;
  }

  MPI::COMM_WORLD.Bcast(&(conf.frame_start),1,MPI::INT,0);
  MPI::COMM_WORLD.Bcast(&(conf.frame_end),  1,MPI::INT,0);
  MPI::COMM_WORLD.Bcast(&(conf.frame_step), 1,MPI::INT,0);
  MPI::COMM_WORLD.Bcast(&(conf.nthreads),   1,MPI::INT,0);

  // if (conf.isRoot())
  //   cout << "===========================" << endl;
  // for (int i=0;i<mpi_size;++i) {
  //   if (i==conf.mpi_rank) {
  //     conf.print();
  //     cout << "===========================" << endl;
  //   }
  //   MPI::COMM_WORLD.Barrier();
  // }
  success=false; // assume failure
  success = stepper(&conf);


  MPI::Finalize(); // must be called by all procs before exiting
  return EXIT_SUCCESS;
}

