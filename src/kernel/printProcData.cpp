#include <iostream> //cout, endl
#include <fstream> //ofstream
#include <iomanip> //setw
#include <vector>
#include <string>
#include <sstream> // ostringstream
#include <mpi.h>

#include "debug.hpp"

using namespace std;
void printProcXYZ( int frame, string tag, vector<float> &x, vector<float> &y, vector<float> &z)
{
  int mpi_rank = MPI::COMM_WORLD.Get_rank();
  int natoms = x.size();

  if (not ((x.size() == y.size()) and (x.size() == z.size()))) {
    if (mpi_rank == 0) {
      cerr << "Error! Vectors passed to printProc not equally sized!" << endl;
      LOC();
    }
    MPI::Finalize(); // must be called by all procs before exiting
    exit(EXIT_FAILURE);
  }

  // create filename
  ostringstream oss;
  oss << "xyz";
  oss << ".proc" << mpi_rank;
  oss << ".natoms" << natoms;
  oss << ".frame" << frame;
  oss << "."   << tag;
  oss << ".dat";
  string fname = oss.str();

  ofstream file(fname);
  for (int i=0;i<natoms;++i) {
    file << setw(10);
    file << x[i];
    file << setw(10);
    file << y[i];
    file << setw(10);
    file << z[i] << endl;
  }
  file.close();
}


