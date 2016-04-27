#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>

class Config 
{
  public:

    int frame_start;
    int frame_end;
    int frame_step;
    int nthreads;

    std::string path;
    std::string xml;
    std::string dcd;
    std::string type1;
    std::string type2;
    std::string kernel_str;

    int mpi_rank;
    int mpi_size;
    int imin;
    int imax;

    bool isRoot() { 
      return (bool)(mpi_rank==0);
    }
};

#endif
