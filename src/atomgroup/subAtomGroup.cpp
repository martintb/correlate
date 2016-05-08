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

  x.resize(indices.size());
  y.resize(indices.size());
  z.resize(indices.size());
  for (int i=0;i<indices.size();i++) {
    x[i] = allX[ indices[i] ];
    y[i] = allY[ indices[i] ];
    z[i] = allZ[ indices[i] ];
  }
}

void subAtomGroup::getTypes(vector<string>&type) {
  vector<string> allType;
  master->getTypes(allType);

  type.resize(indices.size());
  for (int i=0;i<indices.size();i++) {
    type[i] = allType[ indices[i] ];
  }
}

void subAtomGroup::getMolecules(vector<int>&molecule) {
  vector<int> allMolecule;
  master->getMolecules(allMolecule);

  molecule.resize(indices.size());
  for (int i=0;i<indices.size();i++) {
    molecule[i] = allMolecule[ indices[i] ];
  }
}

void subAtomGroup::getBox(vector<float>&box) {
  master->getBox(box);
}

AtomGroup::ptr subAtomGroup::select_types(vector<string> &selTypes) {
  return (master->select_types(selTypes));
}
