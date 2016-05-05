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
    void print(int);
    void contains();
    void sync();
    void print(std::string);
    std::vector<int> readFiles();

    int mpi_rank;
    int mpi_size;

    std::string outfile;
    std::string type1, type2;
    std::string path,topo,trj;
    std::string topoPath,trjPath;


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
      inter_mol_rdf,
      omega,
      inter_mol_omega,
    };

    const std::vector<std::string> KernelMap = 
    { 
      "printProcXYZ",
      "histogram",
      "rdf",
      "inter_mol_rdf",
      "omega",
      "inter_mol_omega",
    };
    bool setKernelFromStr();
    std::string kernelStr;
    KernelType kernel;
    

};

#endif
