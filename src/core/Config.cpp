#include <mpi.h>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
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
  output_freq    = -1;
  type1      = "";
  type2      = "";
  kernelStr  = "";
  selfHist   = false;
  overwrite  = false;
  intra      = false;
  inter      = false;
}

Config::~Config() {
}

void Config::sync()
{
  // ###########################
  // ### TRANSFER INTS/BOOLS ###
  // ###########################
  vector<int> ibuf(11,0);
  if (this->isRoot()) {
    ibuf[0]  = frame_start;
    ibuf[1]  = frame_step;
    ibuf[2]  = frame_end;
    ibuf[3]  = nthreads;
    ibuf[4]  = natoms1;
    ibuf[5]  = natoms2;
    ibuf[6]  = output_freq;
    ibuf[7]  = kernel;
    ibuf[8]  = overwrite;
    ibuf[9]  = intra;
    ibuf[10] = inter;
  }
  MPI::COMM_WORLD.Bcast(&ibuf.front(),ibuf.size(),MPI::INT,0);
  frame_start = ibuf[0];
  frame_step  = ibuf[1];
  frame_end   = ibuf[2];
  nthreads    = ibuf[3];
  natoms1     = ibuf[4];
  natoms2     = ibuf[5];
  output_freq = ibuf[6];
  kernel      = static_cast<KernelType>(ibuf[7]);
  overwrite   = static_cast<bool>(ibuf[8]);
  intra       = static_cast<bool>(ibuf[9]);
  inter       = static_cast<bool>(ibuf[10]);

  // #######################
  // ### TRANSFER FLOATS ###
  // #######################
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
  } else {
    cerr << "Error! dx and xmax must be >0." << endl;
    MPI::Finalize(); // must be called by all procs before exiting
    exit(EXIT_FAILURE);
  }

  // #############################
  // ### TRANSFER FILE STRINGS ###
  // #############################
  string outputStr;
  string trjStr;
  string topoStr;
  if (this->isRoot()) {
    outputStr = output_file->path.string();
    trjStr    = trj_file->path.string();
    topoStr   = topo_file->path.string();
  }
  MPI::COMM_WORLD.Barrier();
  syncString(outputStr);
  syncString(trjStr);
  syncString(topoStr);
  if (not this->isRoot()) {
    output_file = make_shared<outFile>(outputStr,overwrite);
    trj_file    = make_shared<inFile>(trjStr);
    topo_file   = make_shared<inFile>(topoStr);
  }

  // ########################
  // ### TRANSFER STRINGS ###
  // ########################
  syncString(type1);
  syncString(type2);
  syncString(kernelStr);
  this->setKernelFromStr();

  // decide what type of histogram we have
  if (inter) {
    selfHist = false;
  } else {
    selfHist = (type1.compare(type2)==0);
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

bool Config::isRoot() { 
  return (mpi_rank==0);
}

void Config::printHeader(string str) { 
  if (this->isRoot()) {
    cout << "================== ";
    cout << str;
    cout << " ==================";
    cout << endl;
  }
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
  cout << "mpi_rank:    " << mpi_rank             << endl;
  cout << "mpi_size:    " << mpi_size             << endl;
  cout << "topo file:   " << topo_file->path      << endl;
  cout << "trj file:    " << trj_file->path       << endl;
  cout << "output file: " << output_file->path    << endl;
  cout << "output freq: " << output_freq          << endl;
  cout << "overwrite:   " << boolalpha<<overwrite << endl;
  cout << "type1:       " << type1                << endl;
  cout << "type2:       " << type2                << endl;
  cout << "natoms1:     " << natoms1              << endl;
  cout << "natoms2:     " << natoms2              << endl;
  cout << "frame_start: " << frame_start          << endl;
  cout << "frame_end:   " << frame_end            << endl;
  cout << "frame_step:  " << frame_step           << endl;
  cout << "kernel:      " << KernelMap[kernel]    << endl;
  cout << "dx:          " << dx                   << endl;
  cout << "xmax:        " << xmax                 << endl;
  cout << "xsize:       " << xsize                << endl;
  cout << "selfHist:    " << boolalpha<<selfHist  << endl;
  cout << "intra:       " << boolalpha<<intra     << endl;
  cout << "inter:       " << boolalpha<<inter     << endl;
}

void Config::printKernelList() {
  cout << "The following calculation kernels are available: " << endl;
  for (const auto &i : KernelMap) {
    cout << setw(15) << i << endl;
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
    cerr << "Kernel string: " << kernelStr << endl;
    return false;
  }
  return true; //success!
}

