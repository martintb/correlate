#include <mpi.h>
#include <vector>

#include "intWriter.hpp"
#include "types.hpp"

using namespace std;

void intWriter::reset() {
  step_count = 0;
  pair_count = 0;
  pair_count_master = 0;
  box.assign(3,0.0f);
  vecInt.assign(conf->xsize,0);
}


void intWriter::gather() {
  vector<int_type> allVecInt(conf->xsize,0);
  MPI::COMM_WORLD.Reduce(&vecInt.front(),&allVecInt.front(),vecInt.size(),
                       int_mpi_type,MPI::SUM,0);
  vecMaster.assign(allVecInt.begin(),allVecInt.end());

  pair_count_master = 0;
  MPI::COMM_WORLD.Reduce(&pair_count,&pair_count_master,1,int_mpi_type,MPI::SUM,0);
}

