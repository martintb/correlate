#include <mpi.h>
#include <vector>

#include "floatWriter.hpp"

using namespace std;

void floatWriter::reset() {
  step_count = 0;
  pair_count = 0;
  pair_count_master = 0;
  box.assign(3,0.0f);
  vecMaster.assign(conf->xsize,0);
  vecFloat.assign(conf->xsize,0);
  coeffAdd.assign(conf->xsize,0);
  coeffMult.assign(conf->xsize,0);
}


void floatWriter::gather() {
    MPI::COMM_WORLD.Reduce(&vecFloat.front(),&vecMaster.front(),vecFloat.size(),
                         MPI::FLOAT,MPI::SUM,0);
    MPI::COMM_WORLD.Reduce(&pair_count,&pair_count_master,1, MPI::UNSIGNED_LONG,MPI::SUM,0);
}

