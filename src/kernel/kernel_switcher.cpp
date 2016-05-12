#include <mpi.h>
#include <vector>
#include <string>

#include "Config.hpp"
#include "Writer.hpp"
#include "debug.hpp"
#include "kernels.hpp"

using namespace std;

void kernel_switcher( 
                     int frame,
                     Config::ptr &conf,
                     Writer::ptr &writer, 
                     vector<float> &x1, vector<float> &y1, vector<float> &z1,
                     vector<float> &x2, vector<float> &y2, vector<float> &z2,
                     vector<int> &mol1, vector<int> &mol2,
                     vector<float> &box, int offset
                    )
{
                      
  //####################//
  //### printProcXYZ ###//
  //####################//
  if (conf->kernel == Config::printProcXYZ) {
    conf->print("--> Calling kernel: printProcXYZ");
    printProcXYZ(frame,"xyz1",x1,y1,z1);
    printProcXYZ(frame,"xyz2",x2,y2,z2);
    
  //#####################//
  //### histogram/rdf ###//
  //#####################//
  } else if (conf->kernel == Config::histogram or conf->kernel == Config::rdf) {
    conf->print("--> Calling kernel: histogram/rdf");
    histogram(writer->vecInt,
              x1,y1,z1,
              x2,y2,z2,
              box,
              conf->selfHist,
              conf->xmax,
              conf->dx,
              offset,
              writer->pair_count);

  //###########################//
  //### inter_histogram/rdf ###//
  //###########################//
  } else if (conf->kernel == Config::inter_mol_rdf) {
    conf->print("--> Calling kernel: inter_mol_rdf");
    inter_mol_histogram(writer->vecInt,
                        x1,y1,z1,
                        x2,y2,z2,
                        mol1,mol2,
                        box, 
                        conf->xmax,
                        conf->dx,
                        writer->pair_count);

  //#############//
  //### omega ###//
  //#############//
  } else if (conf->kernel == Config::omega) {
    conf->print("--> Calling kernel: omega");
    omega(writer->vecFloat,
          x1,y1,z1,
          x2,y2,z2,
          box,
          conf->selfHist,
          conf->xmax,
          conf->dx,
          offset,
          writer->pair_count);

  //###################//
  //### inter_omega ###//
  //###################//
  } else if (conf->kernel == Config::inter_mol_omega) {
    conf->print("--> Calling kernel: inter_mol_omega");
    inter_mol_omega(writer->vecFloat,
                    x1,y1,z1,
                    x2,y2,z2,
                    mol1,mol2,
                    box, 
                    conf->xmax,
                    conf->dx,
                    writer->pair_count);

  //###################//
  //### intra_omega ###//
  //###################//
  } else if (conf->kernel == Config::intra_mol_omega) {
    conf->print("--> Calling kernel: intra_mol_omega");
    intra_mol_omega(writer->vecFloat,
                    x1,y1,z1,
                    x2,y2,z2,
                    mol1,mol2,
                    box, 
                    conf->xmax,
                    conf->dx,
                    writer->pair_count);

  //##############//
  //### ERROR! ###//
  //##############//
  } else {
    if (conf->isRoot()) {
      cerr << "==> Error! kernel_switcher() is not set up for this kernel." << endl;
      LOC();
    }
    MPI::Finalize();
    exit(EXIT_FAILURE);
  }

  writer->step_count+=1;
  writer->box[0]+=box[0];
  writer->box[1]+=box[1];
  writer->box[2]+=box[2];
  if (writer->step_count == static_cast<unsigned long>(conf->output_freq)) {
    writer->write(false,true);
  }
}
