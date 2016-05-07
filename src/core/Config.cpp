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

  natoms1 = -1;
  natoms2 = -1;
  frame_start = -1;
  frame_end   = -1;
  frame_step  = -1;
  nthreads    = -1;
  dx = -1.0f;
  xmax = -1.0f;
  xsize = 0;
  outfile    = "";
  trjPath    = "";
  topoPath   = "";
  type1      = "";
  type2      = "";
  kernelStr  = "";
  selfHist = false;
}

Config::~Config() {
}

void Config::sync()
{
  vector<int> ibuf(7,0);
  if (this->isRoot()) {
    ibuf[0] = frame_start;
    ibuf[1] = frame_step;
    ibuf[2] = frame_end;
    ibuf[3] = nthreads;
    ibuf[4] = kernel;
    ibuf[5] = natoms1;
    ibuf[6] = natoms2;
  }
  MPI::COMM_WORLD.Bcast(&ibuf.front(),ibuf.size(),MPI::INT,0);
  frame_start = ibuf[0];
  frame_step  = ibuf[1];
  frame_end   = ibuf[2];
  nthreads    = ibuf[3];
  kernel      = static_cast<KernelType>(ibuf[4]);
  natoms1     = ibuf[5];
  natoms2     = ibuf[6];

  vector<float> fbuf(2,0.0f);
  if (this->isRoot()) {
    fbuf[0] = dx;
    fbuf[1] = xmax;
  }
  MPI::COMM_WORLD.Bcast(&fbuf.front(),fbuf.size(),MPI::FLOAT,0);
  dx = fbuf[0];
  xmax  = fbuf[1];
  if (dx>0 and xmax>0) {
    xsize = static_cast<int>(xmax/dx);
  }

  MPI::COMM_WORLD.Barrier();
  syncString(outfile);
  syncString(trjPath);
  syncString(topoPath);
  syncString(type1);
  syncString(type2);
  syncString(kernelStr);

  // decide what type of histogram we have
  if (
      kernel == Config::inter_mol_rdf or
      kernel == Config::inter_mol_omega or
      kernel == Config::intra_mol_omega
     ) 
  {
    selfHist = false;
  } else {
    selfHist = type1.compare(type2)==0;
  }
}

void Config::syncString(string &str) {
  int size=0;
  if (mpi_rank==0) {
    size =str.length();
    MPI::COMM_WORLD.Bcast(&size,1,MPI::CHAR,0);
    MPI::COMM_WORLD.Bcast(const_cast<char*>(str.c_str()),str.length(),MPI::CHAR,0);
  } else {
    MPI::COMM_WORLD.Bcast(&size,1,MPI::CHAR,0);
    char *buf = new char[size];
    MPI::COMM_WORLD.Bcast(buf,size,MPI::CHAR,0);
    str.assign(buf,size);
    delete[] buf;
  }
}

void Config::setTopoFile(string path,string topo) {
  topoPath = path + "/" + topo;
}

void Config::setTrjFile(string path,string trj) {
  trjPath = path + "/" + trj;
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

void Config::print(int proc) { 
  if (this->mpi_rank == proc) {
    this->contains();
  }
  MPI::COMM_WORLD.Barrier();
}

void Config::print() { 
  this->print("===========================");
  for (int i=0;i<this->mpi_size;++i) {
    MPI::COMM_WORLD.Barrier();
    if (i==this->mpi_rank) {
      this->contains();
      cout << "===========================" << endl;
    }
  }
}

void Config::contains() { 
  std::cout << "mpi_rank:    " << mpi_rank    << std::endl;
  std::cout << "mpi_size:    " << mpi_size    << std::endl;
  std::cout << "topo file:   " << topoPath    << std::endl;
  std::cout << "trj file:    " << trjPath     << std::endl;
  std::cout << "type1:       " << type1       << std::endl;
  std::cout << "type2:       " << type2       << std::endl;
  std::cout << "natoms1:     " << natoms1     << std::endl;
  std::cout << "natoms2:     " << natoms2     << std::endl;
  std::cout << "frame_start: " << frame_start << std::endl;
  std::cout << "frame_end:   " << frame_end   << std::endl;
  std::cout << "frame_step:  " << frame_step  << std::endl;
  std::cout << "kernel:      " << KernelMap[kernel]    << std::endl;
  std::cout << "dx:          " << dx                   << std::endl;
  std::cout << "xmax:        " << xmax                 << std::endl;
  std::cout << "xsize:       " << xsize                << std::endl;
  std::cout << "selfHist:    " <<boolalpha<< selfHist  << std::endl;
}

bool Config::setKernelFromStr() {
  if (kernelStr.compare("printProcXYZ")==0) {
    kernel = KernelType::printProcXYZ;
  } else if (kernelStr.compare("histogram")==0) {
    kernel = KernelType::histogram;
  } else if (kernelStr.compare("rdf")==0) {
    kernel = KernelType::rdf;
  } else if (kernelStr.compare("inter_mol_rdf")==0) {
    kernel = KernelType::inter_mol_rdf;
  } else if (kernelStr.compare("omega")==0) {
    kernel = KernelType::omega;
  } else if (kernelStr.compare("inter_mol_omega")==0) {
    kernel = KernelType::inter_mol_omega;
  } else if (kernelStr.compare("intra_mol_omega")==0) {
    kernel = KernelType::intra_mol_omega;
  } else {
    cerr << "Error! Kernel string not recognized." << endl;
    cerr << "Kernel string: " << kernelStr << endl;
    return false;
  }
  return true; //success!
}

