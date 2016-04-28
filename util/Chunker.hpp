#ifndef CHUNKER_HPP
#define CHUNKER_HPP

#include <iostream>
#include <memory>
#include <vector>

class Chunker 
{
  public:
    typedef std::shared_ptr<Chunker> ptr;

    Chunker(int index_count,int num_chunks);
    ~Chunker() {};
    void print();
    void distribute(std::vector<float>*, std::vector<float>*);
                   

    int count;
    int num_chunks;
    std::vector<int> mindex_list;
    std::vector<int> maxdex_list;
    std::vector<int> chunk_sizes;
};
#endif
