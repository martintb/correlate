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
    bool isRoot();
    void print();

    int frame_start;
    int frame_end;
    int frame_step;
    int nthreads;

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
    };

    const std::vector<std::string> KernelMap = { "printProcXYZ" };
    bool setKernelFromStr();
    std::string kernelStr;
    KernelType kernel;
    

};

#endif
