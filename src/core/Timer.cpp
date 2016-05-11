#include <iostream>
#include <iomanip> //setw
#include <map>
#include <cmath> // sqrt
#include <chrono>
#include <string>
#include <vector>

#include <mpi.h>

#include "Timer.hpp"
#include "debug.hpp"

using namespace std;

Timer::Timer()
{
  mpi_size = MPI::COMM_WORLD.Get_size();
  mpi_rank = MPI::COMM_WORLD.Get_rank();
  total_t0 = chrono::high_resolution_clock::now();
}

Timer::~Timer() {};


void Timer::tic(string key) 
{
  if (not t0.count(key)) {
    dt[key] = 0;
    counts[key] = 0;
  }
  t0[key] = chrono::high_resolution_clock::now();
}

void Timer::toc(string key,bool printSplit) 
{
  if (not t0.count(key)) {
    if (mpi_rank==0) {
      cerr << "Error! Tic must be called with a key before toc." << endl;
      LOC();
    }
    MPI::Finalize();
    exit(EXIT_FAILURE);
  }

  Timer::time_point t1 = chrono::high_resolution_clock::now();
  dt[key] += fsecs(t1-t0[key]).count();
  counts[key] += 1;

  if ((mpi_rank==0) and printSplit) {
    cout << ">>> root proc ";
    cout << key;
    cout << " split: ";
    cout << fsecs(t1-t0[key]).count();
    cout << " seconds" << endl;
  };
}

void Timer::print_stats() 
{

  vector<float> avg_buf_in;
  vector<float> avg_sq_buf_in;
  vector<float> tot_buf_in;
  vector<int> count_buf_in;
  for (auto const &kv : dt ) {
    float count = counts[kv.first];
    float tot = kv.second;
    float avg = tot/count;
    avg_buf_in.push_back(avg);
    avg_sq_buf_in.push_back(avg*avg);
    tot_buf_in.push_back(tot);
    count_buf_in.push_back(count);
  }

  vector<float> avg_sq_buf_out(avg_sq_buf_in.size());
  MPI::COMM_WORLD.Reduce(&avg_sq_buf_in.front(),&avg_sq_buf_out.front(),
                         avg_sq_buf_in.size(),MPI::FLOAT,MPI::SUM,0);

  vector<float> avg_buf_out(avg_buf_in.size());
  MPI::COMM_WORLD.Reduce(&avg_buf_in.front(),&avg_buf_out.front(),
                         avg_buf_in.size(),MPI::FLOAT,MPI::SUM,0);

  vector<float> tot_buf_out(tot_buf_in.size());
  MPI::COMM_WORLD.Reduce(&tot_buf_in.front(),&tot_buf_out.front(),
                         tot_buf_in.size(),MPI::FLOAT,MPI::SUM,0);

  vector<int> count_buf_out(count_buf_in.size());
  MPI::COMM_WORLD.Reduce(&count_buf_in.front(),&count_buf_out.front(),
                         count_buf_in.size(),MPI::INT,MPI::SUM,0);


  if (mpi_rank==0) {
    auto t1 = chrono::high_resolution_clock::now();
    float total_time = fsecs(t1-total_t0).count();

    int width=15;
    int key_width=width+5;
    cout << setw(key_width) << "name";
    cout << setw(width) << "per call avg";
    cout << setw(width) << "per call var";
    cout << setw(width) << "total";
    cout << setw(width) << "call count";
    cout << setw(width) << "%";
    cout << endl;
    cout << setw(key_width) << "----";
    cout << setw(width)     << "------------";
    cout << setw(width)     << "------------";
    cout << setw(width)     << "-----";
    cout << setw(width)     << "----------";
    cout << setw(width)     << "-";
    cout << endl;
    int i = 0;
    float N = static_cast<float>(mpi_size);
    for (auto const &kv : dt ) {
      auto key = kv.first;
      float per_call_avg = avg_buf_out[i]/N;
      float per_call_var = avg_sq_buf_out[i]/N - (avg_buf_out[i]/N)*(avg_buf_out[i]/N);
      float total        = tot_buf_out[i]/N;
      float count        = count_buf_out[i]/N;
      float perc         = 100*total/total_time;
      cout << setw(key_width) << key;
      cout << setw(width)    << per_call_avg;
      cout << setw(width)    << per_call_var;
      cout << setw(width)    << total;
      cout << setw(width)    << count;
      cout << setw(width)    << perc;
      cout << endl;
      i++;
    }
  }
  MPI::COMM_WORLD.Barrier();

}


