#ifndef CHUNKER_HPP
#define CHUNKER_HPP

#include <iostream>
#include <memory>
#include <vector>

class Chunker 
{
  public:
    typedef std::shared_ptr<Chunker> ptr;

    Chunker(int index_count,int num_chunks,int chunk_index);
    ~Chunker() {};
    void print();

    int index_count;
    int num_chunks;
    int chunk_index;
    int base_chunk_size;
    int chunk_size;
    int mindex;
    int maxdex;
    std::vector<int> mindex_list;
    std::vector<int> maxdex_list;
};
#endif
