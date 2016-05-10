#include <mpi.h>
#include <vector>
#include <string>

#include "Config.hpp"
#include "debug.hpp"
#include "kernels.hpp"

using namespace std;

void kernel_switcher( 
                     Config *conf,
                     vector<unsigned long> &procVecInt,
                     vector<float> &procVecFloat,
                     vector<float> &x1, vector<float> &y1, vector<float> &z1,
                     vector<float> &x2, vector<float> &y2, vector<float> &z2,
                     vector<int> &mol1, vector<int> &mol2,
                     vector<float> &box, int offset, unsigned long &pair_count,
                     int frame
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
      histogram(procVecInt,
                x1,y1,z1,
                x2,y2,z2,
                box,
                conf->selfHist,
                conf->xmax,conf->dx,
                offset);

    //###########################//
    //### inter_histogram/rdf ###//
    //###########################//
    } else if (conf->kernel == Config::inter_mol_rdf) {
      conf->print("--> Calling kernel: inter_mol_rdf");
      pair_count=0;
      inter_mol_histogram(procVecInt,
                          x1,y1,z1,
                          x2,y2,z2,
                          mol1,mol2,
                          box, conf->xmax,conf->dx,pair_count);

    //#############//
    //### omega ###//
    //#############//
    } else if (conf->kernel == Config::omega) {
      conf->print("--> Calling kernel: omega");
      omega(procVecFloat,
            x1,y1,z1,
            x2,y2,z2,
            box,
            conf->selfHist,
            conf->xmax,conf->dx,
            offset);

    //###################//
    //### inter_omega ###//
    //###################//
    } else if (conf->kernel == Config::inter_mol_omega) {
      conf->print("--> Calling kernel: inter_mol_omega");
      pair_count=0;
      inter_mol_omega(procVecFloat,
                      x1,y1,z1,
                      x2,y2,z2,
                      mol1,mol2,
                      box, conf->xmax,conf->dx,pair_count);

    //###################//
    //### intra_omega ###//
    //###################//
    } else if (conf->kernel == Config::intra_mol_omega) {
      conf->print("--> Calling kernel: intra_mol_omega");
      pair_count=0;
      intra_mol_omega(procVecFloat,
                      x1,y1,z1,
                      x2,y2,z2,
                      mol1,mol2,
                      box, conf->xmax,conf->dx,pair_count);

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
}
