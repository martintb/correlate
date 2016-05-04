#include <iostream>
#include <memory>
#include "Reader.hpp"

using namespace std;

int main(int argc, char* argv[]) 
{
  Reader::ptr rptr = Reader::get("../../data/trajectory.lj.dcd");

  rptr->printFileInfo();
  rptr->printReaderInfo();
  cout << rptr->natoms << endl;

  cout << "POS: "<<boolalpha<<static_cast<bool>(rptr->readsFlag & Reader::POSITION)  << endl;
  cout << "VEL: "<<boolalpha<<static_cast<bool>(rptr->readsFlag & Reader::VELOCITY)  << endl;

  std::vector<float> x;
  std::vector<float> y;
  std::vector<float> z;
  rptr->getPositions(x,y,z);
  cout << x[0] << endl;


  cout << Reader::extInfo["xml"] << endl;

  return EXIT_SUCCESS;
}

