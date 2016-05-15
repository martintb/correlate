#include <mpi.h>
#include <string>
#include <memory>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <chrono> // now()
#include <ctime>  //ctime()
#include <cmath> //PI

#include "debug.hpp"
#include "floatWriter.hpp"
#include "intWriter.hpp"
#include "Config.hpp"
#include "version.hpp"

using namespace std;


Writer::Writer(Config::ptr conf) {
  this->conf = conf;
  file_opened=false;
}

Writer::ptr Writer::get(Config::ptr &conf) {
  if ( conf->kernel == Config::histogram or conf->kernel == Config::rdf) {
    return make_shared<intWriter>(conf);

  } else if ( conf->kernel == Config::omega ) {
    return make_shared<floatWriter>(conf);

  } else {
    cerr << "Error! Writer not fully set up for this kernel!" << endl;
    cerr << "Kernel: " << conf->KernelMap[conf->kernel] << endl;
    LOC();
    MPI::Finalize();
    exit(EXIT_FAILURE);
  }
}


void Writer::write(bool vertical,bool reset) {
  if (not vertical) {
    writeHorizontal(reset);
  } else {
    writeVertical(reset);
  }
}


void Writer::writeVertical(bool reset) {
  gather(); // fill vecMaster with data from all procs
  if (conf->isRoot()) {
    buildCoeff();

    float cutoff;
    if ( conf->kernel == Config::histogram or conf->kernel == Config::rdf) {
      cutoff = box[0]/(step_count*2.0);
      cout << "--> Cutting off data at lx/2.0 = "  << cutoff << endl;
    } else {
      cutoff = conf->xmax;
      cout << "--> Using full data range with xmax = "  << cutoff << endl;
    }

    int width=15;
    ofstream file(conf->output_file->path.string());
    file << "#";
    file << setw(width-1)<< "x";
    file << setw(width)  << conf->KernelMap[conf->kernel];
    file << endl;
    for (int i=0;i<conf->xsize;i++) {
      float x = i*conf->dx;
      if (x<cutoff) {
        file << setw(width) << x;
        file << setw(width) << vecMaster[i]*coeffMult[i] + coeffAdd[i]; 
        file << endl;
      }
    }
    file.close();
  }
  MPI::COMM_WORLD.Barrier();

  if (reset)
    this->reset(); //reset counters and arrays
}


void Writer::writeHeader(int cutoff) {
  ofstream file(conf->output_file->path.string());
  time_t now = chrono::system_clock::to_time_t(chrono::system_clock::now());
  string date = ctime(&now);

  int gcc_major   = __GNUC__;
  int gcc_minor   = __GNUC_MINOR__;
  int gcc_patch   = __GNUC_PATCHLEVEL__;
  int boost_major = BOOST_VERSION/100000;
  int boost_minor = BOOST_VERSION/100 % 1000;
  int boost_patch = BOOST_VERSION % 1000;
  int ompi_major = OMPI_MAJOR_VERSION;
  int ompi_minor = OMPI_MINOR_VERSION;
  int ompi_patch = OMPI_RELEASE_VERSION;

  file << "# --------------------------------------------------------------------------" << endl;
  file << "#                             >>> CORRELATE <<<" << endl;
  file << "# --------------------------------------------------------------------------" << endl;
  file << "# ==> Correlate Version: " << version << endl;
  file << "# ==> GCC Version:       " << gcc_major << "." << gcc_minor << "."<< gcc_patch << endl;
  file << "# ==> Boost Version:     " << boost_major << "." << boost_minor << "."<< boost_patch << endl;
  file << "# ==> OpenMPI Version:   " << ompi_major << "." << ompi_minor << "."<< ompi_patch << endl;
  file << "# ==> Build Date:        " << build_date << endl;
  file << "# ==> File Creation:     " << date; // date already has newline
  file << "# --------------------------------------------------------------------------" << endl;
  file << "# topo file   = " << conf->topo_file->path      << endl;
  file << "# trj file    = " << conf->trj_file->path       << endl;
  file << "# output file = " << conf->output_file->path    << endl;
  file << "# output freq = " << conf->output_freq          << endl;
  file << "# overwrite   = " << boolalpha<<conf->output_freq<< endl;
  file << "# type1       = " << conf->type1                << endl;
  file << "# type2       = " << conf->type2                << endl;
  file << "# natoms1     = " << conf->natoms1              << endl;
  file << "# natoms2     = " << conf->natoms2              << endl;
  file << "# frame_start = " << conf->frame_start          << endl;
  file << "# frame_end   = " << conf->frame_end            << endl;
  file << "# frame_step  = " << conf->frame_step           << endl;
  file << "# dx          = " << conf->dx                   << endl;
  file << "# xmax        = " << conf->xmax                 << endl;
  file << "# xsize       = " << conf->xsize                << endl;
  file << "# selfHist    = " << boolalpha<<conf->selfHist  << endl;
  file << "# kernel      = " << conf->Config::KernelMap[conf->kernel]  << endl;
  file << "# intra       = " << boolalpha<<conf->intra     << endl;
  file << "# inter       = " << boolalpha<<conf->inter     << endl;
  file << "# --------------------------------------------------------------------------" << endl;
  file << "# --> First row below is the grid bin edges (either fourier or real space)" << endl;
  file << "# --> Subsequent rows are data chunks calculated over output_freq timesteps" << endl;
  file << "# --------------------------------------------------------------------------" << endl;

  for (int i=0;i<conf->xsize;i++) {
    float x = i*conf->dx;
    if (x<cutoff) {
      file << x << " ";
    }
  }
  file << endl;
}

void Writer::writeHorizontal(bool reset) {
  gather(); // fill vecMaster with data from all procs
  if (conf->isRoot()) {
    buildCoeff();

    float cutoff;
    if ( conf->kernel == Config::histogram or conf->kernel == Config::rdf) {
      cutoff = box[0]/(step_count*2.0);
      cout << "--> Cutting off data at lx/2.0 = "  << cutoff << endl;
    } else {
      cutoff = conf->xmax;
      cout << "--> Using full data range with xmax = "  << cutoff << endl;
    }

    if (not file_opened) {
      if (not (conf->output_file->exists())) { 
        writeHeader(cutoff); 
      } else if ((conf->overwrite) and (conf->output_file->exists())) { 
        writeHeader(cutoff); 
      }
      file_opened = true;
    }
    ofstream file(conf->output_file->path.string(),std::ofstream::app);
    for (int i=0;i<conf->xsize;i++) {
      float x     = (i*conf->dx);
      float value = (vecMaster[i]*coeffMult[i] + coeffAdd[i]);
      if (x<cutoff) {
        file << value << " ";
      }
    }
    file << endl;
    file.close();
  }
  MPI::COMM_WORLD.Barrier();

  if (reset)
    this->reset(); //reset counters and arrays
}

void Writer::buildCoeff() {
  float tot_natoms;
  if (conf->selfHist) {
    tot_natoms = conf->natoms1;
  } else {
    tot_natoms = conf->natoms1+conf->natoms2;
  }

  // process incremental box data
  pair_count_master /= step_count;
  float lx = box[0]/step_count;
  float ly = box[1]/step_count;
  float lz = box[2]/step_count;
  float box_volume = lx*ly*lz;
  float pair_rho = pair_count_master/box_volume;

  coeffAdd.assign(conf->xsize,0.0f);
  coeffMult.assign(conf->xsize,1.0f);
  for (int i=0;i<conf->xsize;i++) {
    float x1  = (i*conf->dx);
    float x2  = ((i+1)*conf->dx);
    float vol = ((4.0/3.0) * M_PI * (x2*x2*x2  - x1*x1*x1));
    if (conf->kernel == Config::rdf) 
    {
      coeffMult[i]*=(1.0/(step_count*vol*pair_rho));
    } 
    else if ( conf->kernel == Config::omega) {
      if (conf->selfHist) coeffAdd[i] += 1.0;
      coeffMult[i]*=(1.0/(step_count*tot_natoms*x2));
    }
  }
}
