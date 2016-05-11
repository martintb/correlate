#ifndef OMEGA_CPP
#define OMEGA_CPP
#include <vector>
void omega(
                std::vector<float> &hist,
                std::vector<float> &x1, std::vector<float> &y1, std::vector<float> &z1,
                std::vector<float> &x2, std::vector<float> &y2, std::vector<float> &z2,
                std::vector<float> &box, bool selfHist, float qmax, float dq, int offset,
                unsigned long &pair_count
              );
#endif
