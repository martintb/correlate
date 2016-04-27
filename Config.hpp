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
    void chunk(int isize, int num_ichunks, int jsize,int num_jchunks);

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
    std::string kernel_str;

    int mpi_rank;
    int mpi_size;
    Chunker::ptr ichunks;
    Chunker::ptr jchunks;

};

#endif
