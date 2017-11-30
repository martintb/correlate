#include <mpi.h>
#include <functional>
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

  //############################//
  //>>> LAMBDA CONSTRUCTION  <<<//
  //############################//
  // construct comparison lambda
  function<bool (int i, int j)> compare;
  bool selfHist = conf->selfHist;
  //#############//
  //### INTRA ###//
  //#############//
  if (conf->intra) {
    conf->print("--> Constructing intra-molecular lambda");
    compare = [&] (int i,int j) {
      bool test1 = (mol1[i] == mol2[j]); // intra molecular
      bool test2 = (not (selfHist and (i+offset)==j)); // not same bead
      return (test1 and test2);
    };

  //#############//
  //### INTER ###//
  //#############//
  } else if (conf->inter) {
    conf->print("--> Constructing inter-molecular lambda");
    compare = [&] (int i,int j) {
      bool test1 = (mol1[i] != mol2[j]); // inter molecular
      return test1;
    };

  //#############//
  //### FULL ###//
  //#############//
  } else if (
              conf->kernel == Config::histogram or
              conf->kernel == Config::rdf or
              conf->kernel == Config::omega
            )
  {
    conf->print("--> Constructing full lambda");
    compare = [&] (int i,int j) {
      bool test1 = (not (selfHist and (i+offset)==j)); // not same bead
      return test1;
    };
  } else {
  //################//
  //### CATCHALL ###//
  //################//
    conf->print("--> Constructing catch-all lambda (this is probably an error)");
    compare = [] (int i,int j) {
      return true;
    };
  };

  //#########################//
  //>>> KERNEL SWITCHING  <<<//
  //#########################//
                      
  //####################//
  //### printProcData ###//
  //####################//
  if (conf->kernel == Config::procData) {
    conf->print("--> Calling kernel: procData");
    printProcXYZ(frame,"xyz1",x1,y1,z1);
    printProcXYZ(frame,"xyz2",x2,y2,z2);
    
  //#####################//
  //### histogram/rdf ###//
  //#####################//
  } else if (
             conf->kernel == Config::histogram  or
             conf->kernel == Config::rdf
            ) 
  {
    conf->print("--> Calling kernel: histogram/rdf");
    histogram(writer->vecInt,
              writer->pair_count,
              x1,y1,z1,
              x2,y2,z2,
              box,
              conf->xmax, conf->dx,
              compare);

  //#############//
  //### omega ###//
  //#############//
  } else if ( conf->kernel == Config::omega) {
    conf->print("--> Calling kernel: omega");
    omega(writer->vecFloat,
          writer->pair_count,
          x1,y1,z1,
          x2,y2,z2,
          box,
          conf->xmax, conf->dx,
          compare);

  //#############//
  //### msid ###//
  //#############//
  } else if ( conf->kernel == Config::msid) {
    conf->print("--> Calling kernel: msid");
    msid(writer->vecFloat,
          writer->pair_count,
          x1,y1,z1,
          x2,y2,z2,
          box,
          conf->xmax,offset,
          compare);

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

  //###################//
  //>>> DATA OUTPUT <<<//
  //###################//
  writer->step_count+=1;
  writer->box[0]+=box[0];
  writer->box[1]+=box[1];
  writer->box[2]+=box[2];
  if (writer->step_count == static_cast<int_type>(conf->output_freq)) {
    writer->write(/*writeVertical=*/false,/*reset_counters=*/true);
  }
}
