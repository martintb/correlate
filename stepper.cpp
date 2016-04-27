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
  int isize=-1;
  int jsize=-1;
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

    isize = AG1->natoms;
    jsize = AG2->natoms;
  }
  MPI::COMM_WORLD.Barrier(); 



  return true;
}
