#ifndef HISTOGRAM_CPP
#define HISTOGRAM_CPP
#include <functional>
#include <vector>
void histogram(
                std::vector<unsigned long> &hist,
                unsigned long &pair_count,
                const std::vector<float> &x1, const std::vector<float> &y1, const std::vector<float> &z1,
                const std::vector<float> &x2, const std::vector<float> &y2, const std::vector<float> &z2,
                const std::vector<float> &box, 
                const float rmax, const float dr,
                const std::function<bool (int i, int j)>&
              );
#endif
