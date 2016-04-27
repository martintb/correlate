#include <iostream>
#include <memory>
#include <vector>

#include "Chunker.hpp"

Chunker::Chunker(int index_count,int num_chunks,int chunk_index) 
{
  this->index_count = index_count;
  this->num_chunks = num_chunks;
  this->chunk_index = chunk_index;
  base_chunk_size = static_cast<int>(index_count/num_chunks);

  mindex_list.resize(num_chunks);
  maxdex_list.resize(num_chunks);
  for (int i=0;i<num_chunks;++i) {
    mindex_list[i] = i*base_chunk_size;
    maxdex_list[i] = (i+1)*base_chunk_size;
  }
  // need to account for when index_count/num_chunks is not
  // an even number. This makes the "last" chunk a bit larger
  // to handle this
  maxdex_list[num_chunks-1] = index_count;

  mindex = mindex_list[chunk_index];
  maxdex = maxdex_list[chunk_index];
  chunk_size = maxdex-mindex;
};

void Chunker::print() {
  std::cout << "index_count:      " << index_count << std::endl;
  std::cout << "num_chunks:       " << num_chunks << std::endl;
  std::cout << "chunk_index:      " << chunk_index << std::endl;
  std::cout << "base_chunk_size:  " << base_chunk_size << std::endl;
  std::cout << "chunk_size:       " << chunk_size << std::endl;
  std::cout << "mindex:           " << mindex << std::endl;
  std::cout << "maxdex:           " << maxdex << std::endl;

};
