#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include "Reader.hpp"

using namespace std;

int main(int argc, char* argv[]) 
{
  Reader::ptr rptr = Reader::get("../../data/data.lj.lmpbond");

  

  rptr->printFileInfo();
  rptr->printReaderInfo();
  cout << rptr->natoms << endl;

  cout << "POS: "<<boolalpha<<static_cast<bool>(rptr->readsFlag & Reader::POSITION)  << endl;
  cout << "VEL: "<<boolalpha<<static_cast<bool>(rptr->readsFlag & Reader::VELOCITY)  << endl;

  std::vector<float> x;
  std::vector<float> y;
  std::vector<float> z;
  rptr->getPositions(x,y,z);
  cout << x[0]  << " "<< y[0]  << " "<< z[0]  << " " << endl;
  cout << x[10] << " "<< y[10] << " "<< z[10] << " " << endl;
  cout << x[9999] << " "<< y[9999] << " "<< z[9999] << " " << endl;

  std::vector<string> t;
  rptr->getTypes(t);
  cout << t[0] << endl;
  cout << t[10] << endl;
  cout << t[9999] << endl;

  std::vector<int> m;
  rptr->getMolecules(m);
  cout << m[0] << endl;
  cout << m[10] << endl;
  cout << m[9999] << endl;

  std::vector<float> box;
  rptr->getBox(box);
  cout << box[0] << endl;
  cout << box[1] << endl;
  cout << box[2] << endl;

  cout << Reader::extInfo[".lmpbond"] << endl;

  return EXIT_SUCCESS;
}

