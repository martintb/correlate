#include <string>
#include <iostream>
#include <memory>

#include "Config.hpp"
#include "debug.hpp"

using namespace std;

Config::Config() 
{
  mpi_rank  = -1;
  mpi_size  = -1;

  frame_start = -1;
  frame_end   = -1;
  frame_step  = -1;
  nthreads    = -1;

  path       = "";
  xml        = "";
  dcd        = "";
  xmlPath    = "";
  dcdPath    = "";
  type1      = "";
  type2      = "";
  kernel_str = "";
};

Config::~Config() {
};


void Config::chunk(int isize, int num_ichunks, int jsize,int num_jchunks) {
  int iindex,jindex;
  if (num_ichunks==1) {
    iindex = 0;
  } else if (num_ichunks==mpi_size) {
    iindex = mpi_rank;
  } else {
    cerr << "Error! Config.chunks() doesn't know how to handle this num_ichunks." << endl;
    LOC();
    exit(EXIT_FAILURE);
  }
  if (num_jchunks==1) {
    jindex = 0;
  } else {
    cerr << "Error! Config.chunks() doesn't know how to handle this num_jchunks." << endl;
    LOC();
    exit(EXIT_FAILURE);
    jindex = mpi_rank;
  }
      
  ichunks = make_shared<Chunker>(isize,num_ichunks,iindex);
  jchunks = make_shared<Chunker>(jsize,num_jchunks,jindex); // don't chunk the jvalues
}

void Config::buildPaths() {
  xmlPath = path + "/" + xml;
  dcdPath = path+ "/" + dcd;
}


bool Config::isRoot() { 
  return (mpi_rank==0);
}

void Config::print() { 
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
