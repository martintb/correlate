#include <mpi.h>
#include <vector>

#include "floatWriter.hpp"
#include "types.hpp"

using namespace std;

void floatWriter::reset() {
  step_count = 0;
  pair_count = 0;
  pair_count_master = 0;
  box.assign(3,0.0f);
  vecFloat.assign(conf->xsize,0.0f);
}


void floatWriter::gather() {
  vector<float_type> allVecFloat(conf->xsize,0);
  MPI::COMM_WORLD.Reduce(&vecFloat.front(),&allVecFloat.front(),vecFloat.size(),
                       float_mpi_type,MPI::SUM,0);
  vecMaster.assign(allVecFloat.begin(),allVecFloat.end());

  pair_count_master = 0;
  MPI::COMM_WORLD.Reduce(&pair_count,&pair_count_master,1, int_mpi_type,MPI::SUM,0);
}

