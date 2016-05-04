#ifndef INTER_MOL_OMEGA_CPP
#define INTER_MOL_OMEGA_CPP
#include <vector>
void inter_mol_omega(
                std::vector<float> &hist,
                std::vector<float> &x1, std::vector<float> &y1, std::vector<float> &z1,
                std::vector<float> &x2, std::vector<float> &y2, std::vector<float> &z2,
                std::vector<int> &mol1, std::vector<int> &mol2,
                std::vector<float> &box, float qmax, float dq, unsigned long &pair_count
              );
#endif
