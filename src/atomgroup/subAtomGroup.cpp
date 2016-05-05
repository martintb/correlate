#include <iostream>
#include <string>
#include <vector>

#include "Reader.hpp"
#include "AtomGroup.hpp"
#include "masterAtomGroup.hpp"
#include "subAtomGroup.hpp"

using namespace std;

subAtomGroup::subAtomGroup(AtomGroup::ptr master,vector<int> indices)
{
  this->master = master;
  this->indices = indices;
  numFrames = master->numFrames;
  natoms = indices.size();
}


// Similar virtuals to Reader class
void subAtomGroup::readFrame(int frame) {
  master->readFrame(frame);
  this->frame = frame;
}

void subAtomGroup::getPositions(vector<float>&x,vector<float>&y,vector<float>&z) {

  vector<float> allX;
  vector<float> allY;
  vector<float> allZ;
  master->getPositions(allX,allY,allZ);

  x.clear();
  y.clear();
  z.clear();
  x.reserve(indices.size());
  y.reserve(indices.size());
  z.reserve(indices.size());
  for (const auto &i : indices) {
    x.push_back(allX[i]);
    y.push_back(allY[i]);
    z.push_back(allZ[i]);
  }
}

void subAtomGroup::getTypes(vector<string>&type) {
  vector<string> allType;
  master->getTypes(allType);

  type.clear();
  type.reserve(indices.size());
  for (const auto &i : indices) {
    type.push_back(allType[i]);
  }
}

void subAtomGroup::getMolecules(vector<int>&molecule) {
  vector<int> allMolecule;
  master->getMolecules(allMolecule);

  molecule.clear();
  molecule.reserve(indices.size());
  for (const auto &i : indices) {
    molecule.push_back(allMolecule[i]);
  }
}

void subAtomGroup::getBox(vector<float>&box) {
  master->getBox(box);
}

AtomGroup::ptr subAtomGroup::select_types(vector<string> &selTypes) {
  return (master->select_types(selTypes));
}
