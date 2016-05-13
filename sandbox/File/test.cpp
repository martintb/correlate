#include <string>
#include <mpi.h>

#include "File.hpp"
#include "inFile.hpp"
#include "outFile.hpp"

using namespace std;

int main() 
{
  File::ptr fptr  =  make_shared<outFile>("test_set","topo.dat",false);
  return EXIT_SUCCESS;
}

