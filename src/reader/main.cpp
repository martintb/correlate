#include <iostream>
#include <vector>
#include <string>
#include <memory>

#include "AtomGroup.hpp"

using namespace std;

int main(int argc, char* argv[]) 
{
  AtomGroup::ptr AG = AtomGroup::make("../../data/data.PNC.lmpbond","../../data/trajectory.PNC.dcd");

  vector<string> types;
  AG->getTypes(types);
  cout << types[0] << endl;
  cout << types[1] << endl;
  cout << types[2] << endl;

  vector<string> sel = {"1","2"};
  AtomGroup::ptr AG1 = AG->select_types(sel);
  cout << AG1->natoms << endl;

  vector<float> x,y,z;
  AG1->getPositions(x,y,z);
  cout << x[0]  << " " << y[0]  << " " << z[0] << endl;
  cout << x[1]  << " " << y[1]  << " " << z[1] << endl;
  cout << x[10] << " " << y[10] << " " << z[10] << endl;

  return EXIT_SUCCESS;
}

