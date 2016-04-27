#include <string>
#include <mpi.h>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "Config.hpp"
#include "parse_opts.hpp"

using namespace std;

int main(int argc, char* argv[]) 
{
  MPI_Init(NULL,NULL);

  int mpi_size,mpi_rank;
  MPI_Comm_size(MPI_COMM_WORLD,&mpi_size);
  MPI_Comm_rank(MPI_COMM_WORLD,&mpi_rank);

  Config conf;
  conf.mpi_rank = mpi_rank;
  conf.mpi_size = mpi_size;

  bool success=false;
  if (conf.isRoot()) {
    success = parse_opts(argc,argv,&conf);
  }

  if (not success) {
    MPI_Finalize();
    return EXIT_FAILURE;
  }

  MPI_Finalize();
  return EXIT_SUCCESS;
}

