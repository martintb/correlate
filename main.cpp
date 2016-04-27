#include <string>
#include <mpi.h>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "Config.hpp"
#include "parse_opts.hpp"

using namespace std;

int main(int argc, char* argv[]) 
{
  MPI::Init();

  int mpi_size = MPI::COMM_WORLD.Get_size();
  int mpi_rank = MPI::COMM_WORLD.Get_rank();

  // MPI_Comm_size(MPI::COMM_WORLD,&mpi_size);
  // MPI_Comm_rank(MPI::COMM_WORLD,&mpi_rank);

  Config conf;
  conf.mpi_rank = mpi_rank;
  conf.mpi_size = mpi_size;

  bool success=false;
  if (conf.isRoot()) {
    success = parse_opts(argc,argv,&conf);
  } //else {
    // conf.path        = vm["path"].as<string>();
    // conf.xml = vm["xml"].as<string>();
    // conf.dcd = vm["dcd"].as<string>();
    // conf.type1 = vm["type1"].as<string>();
    // conf.type2 = vm["type2"].as<string>();
    // conf.frame_start = vm["frame_start"].as<int>();
    // conf.frame_end   = vm["frame_end"].as<int>();
    // conf.frame_step  = vm["frame_step"].as<int>();
    // conf.nthreads    = vm["nthreads"].as<int>();
  //}
  MPI::COMM_WORLD.Barrier();

  MPI::COMM_WORLD.Bcast(&success,1,MPI::BOOL,0);
  if (not success) {
    MPI::Finalize();
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


  MPI::Finalize();
  return EXIT_SUCCESS;
}

