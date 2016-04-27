#ifndef CHUNKER_HPP
#define CHUNKER_HPP

#include <memory>
#include <vector>

class Chunker 
{
  public:
    typedef std::shared_ptr<Chunker> ptr;

    Chunker(int index_count,int num_chunks,int chunk_index) {
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
      maxdex = mindex_list[chunk_index];
      chunk_size = maxdex-mindex;
    };

    ~Chunker() {};
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
