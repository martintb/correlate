#ifndef HISTOGRAM_CPP
#define HISTOGRAM_CPP
#include <vector>
void histogram(
                std::vector<unsigned long> &hist,
                std::vector<float> &x1, std::vector<float> &y1, std::vector<float> &z1,
                std::vector<float> &x2, std::vector<float> &y2, std::vector<float> &z2,
                std::vector<float> &box, bool selfHist, float rmax, float dr, int offset
              );
#endif
