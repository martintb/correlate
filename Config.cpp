#include <mpi.h>
#include <string>
#include <vector>
#include <iostream>
#include <memory>

#include "Config.hpp"
#include "debug.hpp"

using namespace std;

Config::Config() 
{
  mpi_size = MPI::COMM_WORLD.Get_size();
  mpi_rank = MPI::COMM_WORLD.Get_rank();

  frame_start = -1;
  frame_end   = -1;
  frame_step  = -1;
  nthreads    = -1;

  dr = -1.0f;
  rmax = -1.0f;

  path       = "";
  xml        = "";
  dcd        = "";
  xmlPath    = "";
  dcdPath    = "";
  type1      = "";
  type2      = "";
  kernelStr = "";
}

Config::~Config() {
}

void Config::sync()
{
  vector<int> ibuf(5,0);
  if (this->isRoot()) {
    ibuf[0] = frame_start;
    ibuf[1] = frame_step;
    ibuf[2] = frame_end;
    ibuf[3] = nthreads;
    ibuf[4] = kernel;
  }
  MPI::COMM_WORLD.Bcast(&ibuf.front(),ibuf.size(),MPI::INT,0);
  frame_start = ibuf[0];
  frame_step  = ibuf[1];
  frame_end   = ibuf[2];
  nthreads    = ibuf[3];
  kernel      = static_cast<KernelType>(ibuf[4]);

  vector<float> fbuf(2,0.0f);
  if (this->isRoot()) {
    fbuf[0] = dr;
    fbuf[1] = rmax;
  }
  MPI::COMM_WORLD.Bcast(&fbuf.front(),fbuf.size(),MPI::FLOAT,0);
  dr = fbuf[0];
  rmax  = fbuf[1];
}

bool Config::setKernelFromStr() {
  if (kernelStr.compare("printProcXYZ")==0) {
    kernel = KernelType::printProcXYZ;
  } else if (kernelStr.compare("histogram")==0) {
    kernel = KernelType::histogram;
  } else {
    cerr << "Error! Kernel string not recognized." << endl;
    return false;
  }
  return true; //success!
}


void Config::buildSpace() {
  if (dr>0 and rmax>0) {
    rsize = static_cast<int>(rmax/dr);
  }
}

void Config::buildPaths() {
  xmlPath = path + "/" + xml;
  dcdPath = path+ "/" + dcd;
}


bool Config::isRoot() { 
  return (mpi_rank==0);
}

void Config::print(string str) { 
  if (this->isRoot()) {
    cout << str << endl;
  }
  return;
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
  std::cout << "dr:          " << dr << std::endl;
  std::cout << "rmax:        " << rmax   << std::endl;
}
