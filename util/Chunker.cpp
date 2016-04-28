#include <mpi.h>
#include <iostream>
#include <memory>
#include <vector>

#include "Chunker.hpp"

using namespace std;

Chunker::Chunker(int count,int num_chunks) 
{
  this->count = count;
  this->num_chunks = num_chunks;
  int base_chunk_size = static_cast<int>(count/num_chunks);

  mindex_list.resize(num_chunks);
  maxdex_list.resize(num_chunks);
  chunk_sizes.resize(num_chunks);
  for (int i=0;i<num_chunks;++i) {
    mindex_list[i] = i*base_chunk_size;
    if (i==(num_chunks-1)) {
      maxdex_list[i] = count;
    } else {
      maxdex_list[i] = (i+1)*base_chunk_size;
    }
    chunk_sizes[i] = maxdex_list[i] - mindex_list[i];
  }

};

void Chunker::print() {
  cout << "count:      " << count << endl;
  cout << "num_chunks:       " << num_chunks << endl;
  cout << "rank\tmindex\tmaxdex\tchunk_size" << endl;
  cout << "----\t------\t------\t----------" << endl;
  for (int i=0;i<num_chunks;i++) {
    cout << i << "\t";
    cout << mindex_list[i] << "\t";
    cout << maxdex_list[i] << "\t";
    cout << chunk_sizes[i] << endl;
  }
};


void Chunker::distribute( vector<float> *xin, vector<float> *xout)
{
  int mpi_rank = MPI::COMM_WORLD.Get_rank();
  int mpi_size = MPI::COMM_WORLD.Get_size();
  int chunk_size = chunk_sizes[mpi_rank];
  xout->resize(chunk_size);
  MPI::COMM_WORLD.Scatterv(
                            &(xin->front()),
                            &chunk_sizes.front(), //size of each chunk
                            &mindex_list.front(), //array offsets
                            MPI::FLOAT,
                            &(xout->front()),
                            chunk_size,
                            MPI::FLOAT,
                            0
                          );
  
};

