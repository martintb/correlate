#include <memory>
#include <iostream>
#include <mpi.h>
#include <boost/algorithm/string/classification.hpp> 
#include <boost/algorithm/string/split.hpp>
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

#include "Config.hpp"
#include "AtomGroup.hpp"

using namespace std;

bool stepper(Config *conf) {
  int natoms1=-1,natoms2=-1;
  AtomGroup::ptr AG(nullptr),AG1(nullptr),AG2(nullptr); // only root will have initalized AG
  if (conf->isRoot()) {
    conf->buildPaths();
    AG = make_shared<AtomGroup>(conf->xmlPath,conf->dcdPath);

    AG->readFiles();
    cout << "===================================" << endl;
    AG->xptr->contains();
    AG->xptr->printFileInfo();
    cout << "===================================" << endl;
    AG->dptr->contains();
    AG->dptr->printFileInfo();
    cout << "===================================" << endl;

    vector<string> sel1,sel2;
    boost::split(sel1,conf->type1,boost::is_any_of(", "),boost::token_compress_on);
    boost::split(sel2,conf->type2,boost::is_any_of(", "),boost::token_compress_on);

    AG1 = AG->select(sel1);
    AG2 = AG->select(sel2);

    natoms1 = AG1->natoms;
    natoms2 = AG2->natoms;
  }
  MPI::COMM_WORLD.Bcast(&natoms1,1,MPI::INT,0);
  MPI::COMM_WORLD.Bcast(&natoms2,1,MPI::INT,0);
  conf->chunk(natoms1,conf->mpi_size,natoms2,1);
  for (int i=0;i<conf->mpi_size;i++) {
    MPI::COMM_WORLD.Barrier();
    if (i==conf->mpi_rank) {
      cout << i << "===================================" << endl;
      conf->ichunks->print();
      cout << i << "===================================" << endl;
    }
  }



  return true;
}
