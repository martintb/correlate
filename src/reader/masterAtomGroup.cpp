#include <iostream>
#include <string>
#include <vector>

#include "Reader.hpp"
#include "masterAtomGroup.hpp"

using namespace std;

masterAtomGroup::masterAtomGroup(string topoFile,string trjFile,bool printFileInfo) 
{
  this->topoFile = topoFile;
  this->trjFile = trjFile;

  topo = Reader::get(topoFile);
  trj = Reader::get(trjFile);
  if (printFileInfo) {
    topo->printReaderInfo();
    topo->printFileInfo();
    cout << "-----------------------------" << endl;
    trj->printReaderInfo();
    trj->printFileInfo();
  }



}

// Similar virtuals to Reader class
void masterAtomGroup::readFrame(int) {
  topo->readFrame(frame);
}

void masterAtomGroup::getPositions(std::vector<float>&x,std::vector<float>&y,std::vector<float>&z) {
  trj->getPositions(x,y,z);
}

void masterAtomGroup::getTypes(std::vector<std::string>&type) {
  topo->getTypes(type);
}

void masterAtomGroup::getMolecules(std::vector<int>&molecule) {
  topo->getMolecules(molecule);
}

void masterAtomGroup::getBox(std::vector<float>&box) {
  trj->getBox(box);
}
