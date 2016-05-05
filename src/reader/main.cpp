#include <iostream>
#include <vector>
#include <string>
#include <memory>

#include "AtomGroup.hpp"

using namespace std;

int main(int argc, char* argv[]) 
{
  AtomGroup::ptr AG = AtomGroup::make("../../data/data.lj.lmpbond","../../data/trajectory.lj.dcd");

  vector<string> types;
  AG->getTypes(types);
  cout << types[0] << endl;
  cout << types[1] << endl;
  cout << types[2] << endl;


  return EXIT_SUCCESS;
}

