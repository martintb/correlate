#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <memory>
#include "Chunker.hpp"
#include "File.hpp"
#include "inFile.hpp"
#include "outFile.hpp"

class Config 
{
  public:
    typedef std::shared_ptr<Config> ptr;
    Config();
    ~Config();
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

    std::string type1, type2;
    File::ptr topo_file;
    File::ptr trj_file;
    File::ptr output_file;
    int output_freq;
    int overwrite;

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
    bool inter;
    bool intra;


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
    void printKernelList();
    KernelType kernel;
    

};

#endif
