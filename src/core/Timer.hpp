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
    void tic(std::string);
    void toc(std::string);
    void print_stats();

  private:
    std::map<std::string,time_point> t0;
    std::map<std::string,float> dt;
    std::map<std::string,long> counts;

};

#endif
