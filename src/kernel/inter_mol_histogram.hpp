#ifndef INTER_MOL_HISTOGRAM_CPP
#define INTER_MOL_HISTOGRAM_CPP
#include <vector>
void inter_mol_histogram(
                std::vector<unsigned long> &hist,
                std::vector<float> &x1, std::vector<float> &y1, std::vector<float> &z1,
                std::vector<float> &x2, std::vector<float> &y2, std::vector<float> &z2,
                std::vector<int> &mol1, std::vector<int> &mol2,
                std::vector<float> &box, float rmax, float dr,
                unsigned long &pair_count
              );
#endif
