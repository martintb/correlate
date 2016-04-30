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

  dx = -1.0f;
  xmax = -1.0f;

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
    fbuf[0] = dx;
    fbuf[1] = xmax;
  }
  MPI::COMM_WORLD.Bcast(&fbuf.front(),fbuf.size(),MPI::FLOAT,0);
  dx = fbuf[0];
  xmax  = fbuf[1];
}


void Config::buildSpace() {
  if (dx>0 and xmax>0) {
    xsize = static_cast<int>(xmax/dx);
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
  this->print("===========================");
  for (int i=0;i<this->mpi_size;++i) {
    MPI::COMM_WORLD.Barrier();
    if (i==this->mpi_rank) {
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
      std::cout << "dx:          " << dx << std::endl;
      std::cout << "xmax:        " << xmax   << std::endl;
      cout << "===========================" << endl;
    }
  }
}

bool Config::setKernelFromStr() {
  if (kernelStr.compare("printProcXYZ")==0) {
    kernel = KernelType::printProcXYZ;
  } else if (kernelStr.compare("histogram")==0) {
    kernel = KernelType::histogram;
  } else if (kernelStr.compare("rdf")==0) {
    kernel = KernelType::rdf;
  } else if (kernelStr.compare("omega")==0) {
    kernel = KernelType::omega;
  } else {
    cerr << "Error! Kernel string not recognized." << endl;
    return false;
  }
  return true; //success!
}

