#include <mpi.h>
#include <iostream>

using namespace std;


void getDeviceInfo(int mpi_rank, int mpi_size) ;
int main() 
{
  MPI::Init();
  int mpi_size = MPI::COMM_WORLD.Get_size();
  int mpi_rank = MPI::COMM_WORLD.Get_rank();

  getDeviceInfo(mpi_rank,mpi_size);

  MPI::Finalize(); // must be called by all procs before exiting
  return EXIT_SUCCESS;
}

