#include <stdio.h>
#include <cstdlib>
#include <cuda_runtime.h>

using namespace std;

#define gpuErrchk(ans) { gpuAssert((ans), __FILE__, __LINE__); }
inline void gpuAssert(cudaError_t code, const char *file, int line, bool abort=true)
{
   if (code != cudaSuccess) 
   {
      fprintf(stderr,"GPUassert: %s %s %d\n", cudaGetErrorString(code), file, line);
      if (abort) exit(code);
   }
}

void getDeviceInfo(int mpi_rank, int mpi_size) 
{

  int nDevices=0;
  gpuErrchk(cudaGetDeviceCount(&nDevices));
  printf("(%d/%d) Found %d GPUS\n",mpi_rank,mpi_size,nDevices);
  for (int i = 0; i < nDevices; i++) {
    cudaDeviceProp prop;
    cudaGetDeviceProperties(&prop, i);
    printf("(%d/%d) Device Number: %d\n", mpi_rank,mpi_size,i);
    printf("(%d/%d)   Device name: %s\n", mpi_rank,mpi_size,prop.name);
    printf("(%d/%d)   Memory Clock Rate (KHz): %d\n",
           mpi_rank,mpi_size,prop.memoryClockRate);
    printf("(%d/%d)   Memory Bus Width (bits): %d\n",
           mpi_rank,mpi_size,prop.memoryBusWidth);
    printf("(%d/%d)   Peak Memory Bandwidth (GB/s): %f\n\n",
           mpi_rank,mpi_size,2.0*prop.memoryClockRate*(prop.memoryBusWidth/8)/1.0e6);
  }

}

