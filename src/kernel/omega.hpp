#ifndef OMEGA_CPP
#define OMEGA_CPP
#include <functional>
#include <vector>
#include "types.hpp"

void omega(
                std::vector<float> &hist,
                count_t &pair_count,
                const std::vector<float> &x1, const std::vector<float> &y1, const std::vector<float> &z1,
                const std::vector<float> &x2, const std::vector<float> &y2, const std::vector<float> &z2,
                const std::vector<float> &box,
                const float qmax, const float dq,
                const std::function<bool (int i, int j)>&
              );
#endif
