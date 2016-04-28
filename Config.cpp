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
  kernelStr = "";
};

Config::~Config() {
};

bool Config::setKernelFromStr() {
  if (kernelStr.compare("printProcXYZ")==0) {
    kernel = KernelType::printProcXYZ;
  } else {
    cerr << "Error! Kernel string not recognized." << endl;
    return false;
  }

  return true; //success!
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
  std::cout << "kernel:      " << KernelMap[kernel]  << std::endl;
}
