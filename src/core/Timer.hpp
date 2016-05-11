#ifndef TIMER_HPP
#define TIMER_HPP

#include <map>
#include <chrono>
#include <string>


class Timer
{
  public:
    Timer();
    ~Timer();

    int mpi_rank;
    int mpi_size;

    typedef std::chrono::high_resolution_clock::time_point time_point;
    typedef std::chrono::duration<float,std::ratio<60>> fmins;
    typedef std::chrono::duration<float,std::ratio<1>> fsecs;
    void tic(std::string);
    void toc(std::string,bool printSplit=false);
    void print_stats();

  private:
    std::map<std::string,time_point> t0;
    std::map<std::string,float> dt;
    std::map<std::string,long> counts;
    time_point total_t0;

};

#endif
