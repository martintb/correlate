#ifndef PRINT_PROC_DATA_HPP
#define PRINT_PROC_DATA_HPP

#include <string>
#include <vector>

void printProcXYZ( int frame, 
                   std::string tag, 
                   std::vector<float> &x, 
                   std::vector<float> &y, 
                   std::vector<float> &z);
#endif
