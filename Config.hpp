#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include "Chunker.hpp"

class Config 
{
  public:
    Config();
    ~Config();
    void buildPaths();
    void buildSpace();
    bool isRoot();
    void print();

    int frame_start;
    int frame_end;
    int frame_step;
    int nthreads;

    float dr;
    float rmax;
    int rsize;

    std::string path;
    std::string xml;
    std::string dcd;
    std::string xmlPath;
    std::string dcdPath;
    std::string type1;
    std::string type2;

    int mpi_rank;
    int mpi_size;

    enum KernelType {
      printProcXYZ,
      histogram,
    };

    const std::vector<std::string> KernelMap = 
    { 
      "printProcXYZ",
      "histogram",
    };
    bool setKernelFromStr();
    std::string kernelStr;
    KernelType kernel;
    

};

#endif
