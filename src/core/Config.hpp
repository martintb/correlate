#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <memory>
#include "Chunker.hpp"

class Config 
{
  public:
    typedef std::shared_ptr<Config> ptr;
    Config();
    ~Config();
    void setTopoFile(std::string,std::string);
    void setTrjFile(std::string,std::string);
    void buildSpace();
    bool isRoot();
    void contains();
    void sync();
    void syncString(std::string&);
    std::vector<int> readFiles();

    void print();
    void print(int);
    void print(std::string);
    void printHeader(std::string);

    int mpi_rank;
    int mpi_size;

    std::string output_file;
    int output_freq;
    std::string type1, type2;
    std::string topoPath,trjPath;

    int natoms1;
    int natoms2;
    int frame_start;
    int frame_end;
    int frame_step;
    int nthreads;
    float dx;
    float xmax;
    int xsize;
    bool selfHist;


    enum KernelType {
      printProcXYZ,
      histogram,
      rdf,
      inter_mol_rdf,
      omega,
      inter_mol_omega,
      intra_mol_omega,
    };

    const std::vector<std::string> KernelMap = 
    { 
      "printProcXYZ",
      "histogram",
      "rdf",
      "inter_mol_rdf",
      "omega",
      "inter_mol_omega",
      "intra_mol_omega",
    };
    bool setKernelFromStr();
    std::string kernelStr;
    void printKernelList();
    KernelType kernel;
    

};

#endif
