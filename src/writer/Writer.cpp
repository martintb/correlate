#include <mpi.h>
#include <string>
#include <memory>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>

#include "debug.hpp"
#include "floatWriter.hpp"
#include "intWriter.hpp"
#include "Config.hpp"

using namespace std;


Writer::Writer(Config::ptr conf) {
  this->conf = conf;
}

Writer::ptr Writer::get(Config::ptr &conf) {
  if (
      conf->kernel == Config::histogram or 
      conf->kernel == Config::rdf or
      conf->kernel == Config::inter_mol_rdf
     ) 
  {
    return make_shared<intWriter>(conf);
  } else if (
              conf->kernel == Config::omega or
              conf->kernel == Config::inter_mol_omega or
              conf->kernel == Config::intra_mol_omega
            ) 
  {
    return make_shared<floatWriter>(conf);
  } else {
    cerr << "Error! Writer not fully set up for this kernel!" << endl;
    cerr << "Kernel: " << conf->KernelMap[conf->kernel] << endl;
    LOC();
    MPI::Finalize();
    exit(EXIT_FAILURE);
  }
}

void Writer::buildCoeff() {
  float tot_natoms = conf->natoms1+conf->natoms2;

  // process incremental box data
  float lx = box[0]/step_count;
  float ly = box[1]/step_count;
  float lz = box[2]/step_count;
  float box_volume = lx*ly*lz;
  float pair_rho = pair_count_master/box_volume/step_count;

  coeffAdd.assign(conf->xsize,0.0f);
  coeffMult.assign(conf->xsize,1.0f);
  for (int i=0;i<conf->xsize;i++) {
    float x1 = i*conf->dx;
    float x2 = (i+1)*conf->dx;
    float vol = (4.0/3.0) * (3.14159) * (x2*x2*x2  - x1*x1*x1);
    if (conf->kernel == Config::rdf) 
    {
      coeffMult[i]*=1.0/(step_count*vol*pair_rho);
    } 
    else if (
               conf->kernel == Config::omega or
               conf->kernel == Config::inter_mol_omega or
               conf->kernel == Config::intra_mol_omega
              )
    {
      if (conf->selfHist) coeffAdd[i] += 1.0;
      coeffMult[i]*=1.0/(step_count*tot_natoms*x2);
    }
  }
}

void Writer::write() {
  gather(); // fill vecMaster with data from all procs
  if (conf->isRoot()) {
    buildCoeff();

    float cutoff;
    if (
        conf->kernel == Config::histogram or 
        conf->kernel == Config::rdf or
        conf->kernel == Config::inter_mol_rdf
       )
    {
      cutoff = box[0]/(step_count*2.0);
      cout << "--> Cutting off data at lx/2.0 = "  << cutoff << endl;
    } else {
      cutoff = conf->xmax;
      cout << "--> Using full data range with xmax = "  << cutoff << endl;
    }

    int width=15;
    ofstream file(conf->outfile);
    file << "#";
    file << setw(width-1)<< "x";
    file << setw(width)  << conf->KernelMap[conf->kernel];
    file << endl;
    for (int i=0;i<conf->xsize;i++) {
      file << setw(width) << (i*conf->dx);
      file << setw(width) << vecMaster[i]*coeffMult[i] + coeffAdd[i]; 
      file << endl;
    }
    file.close();
  }
  MPI::COMM_WORLD.Barrier();

  this->reset(); //reset counters and arrays
}

