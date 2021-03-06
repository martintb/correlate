#ifndef KERNEL_SWITCHER_HPP
#define KERNEL_SWITCHER_HPP

#include <vector>
#include "Config.hpp"
#include "Writer.hpp"

void kernel_switcher( 
                     int frame,
                     Config::ptr &conf,
                     Writer::ptr &writer,
                     std::vector<float> &x1, std::vector<float> &y1, std::vector<float> &z1,
                     std::vector<float> &x2, std::vector<float> &y2, std::vector<float> &z2,
                     std::vector<int> &mol1, std::vector<int> &mol2,
                     std::vector<float> &box, int offset
                    );
#endif
