#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <iostream>

class Config 
{
  public:
    Config() {
      frame_start = -1;
      frame_end   = -1;
      frame_step  = -1;
      nthreads    = -1;

      path       = "";
      xml        = "";
      dcd        = "";
      type1      = "";
      type2      = "";
      kernel_str  = "";

      mpi_rank  = -1;
      mpi_size  = -1;
      imin  = -1;
      imax  = -1;
    };
    ~Config() {};

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

    void print() { 
      std::cout << "mpi_rank:    " << mpi_rank    << std::endl;
      std::cout << "mpi_size:    " << mpi_size    << std::endl;
      std::cout << "xml:         " << xml         << std::endl;
      std::cout << "dcd:         " << dcd         << std::endl;
      std::cout << "type1:       " << type1       << std::endl;
      std::cout << "type2:       " << type2       << std::endl;
      std::cout << "frame_start: " << frame_start << std::endl;
      std::cout << "frame_end:   " << frame_end   << std::endl;
      std::cout << "frame_step:  " << frame_step  << std::endl;
    }
};

#endif
