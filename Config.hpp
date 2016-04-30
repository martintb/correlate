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
    void sync();
    void print(std::string);
    std::vector<int> readFiles();

    int mpi_rank;
    int mpi_size;

    std::string type1, type2;
    std::string path,xml,dcd;
    std::string xmlPath,dcdPath;


    int frame_start;
    int frame_end;
    int frame_step;
    int nthreads;

    // output data size
    float dx;
    float xmax;
    int xsize;


    enum KernelType {
      printProcXYZ,
      histogram,
      rdf,
      omega,
    };

    const std::vector<std::string> KernelMap = 
    { 
      "printProcXYZ",
      "histogram",
      "rdf",
      "omega",
    };
    bool setKernelFromStr();
    std::string kernelStr;
    KernelType kernel;
    

};

#endif
