#ifndef MSID_CPP
#define MSID_CPP
#include <functional>
#include <vector>
void msid(
                std::vector<float> &hist,
                unsigned long &pair_count,
                const std::vector<float> &x1, const std::vector<float> &y1, const std::vector<float> &z1,
                const std::vector<float> &x2, const std::vector<float> &y2, const std::vector<float> &z2,
                const std::vector<float> &box,
                const float xmax,const int offset,
                const std::function<bool (int i, int j)>&
              );
#endif
