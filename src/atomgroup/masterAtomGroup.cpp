#include <set>// set
#include <algorithm>// sort
#include <iostream>
#include <string>
#include <vector>

#include "debug.hpp"
#include "Reader.hpp"
#include "AtomGroup.hpp"
#include "masterAtomGroup.hpp"
#include "subAtomGroup.hpp"

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

  numFrames = trj->numFrames;
  natoms = trj->natoms;
  if (trj->natoms != topo->natoms) {
    cerr << "Error! Topolgy and trajectory files have different numbers of atoms:" << endl;
    cerr << "Topology natoms: " << topo->natoms << endl;
    cerr << "Trajectory natoms: " << trj->natoms << endl;
    LOC();
    exit(EXIT_FAILURE);
  }
}


// Similar virtuals to Reader class
void masterAtomGroup::readFrame(int frame) {
  trj->readFrame(frame);
  this->frame = frame;
}

void masterAtomGroup::getPositions(vector<float>&x,vector<float>&y,vector<float>&z) {
  trj->getPositions(x,y,z);
}

void masterAtomGroup::getTypes(vector<string>&type) {
  topo->getTypes(type);
}

void masterAtomGroup::getMolecules(vector<int>&molecule) {
  topo->getMolecules(molecule);
}

void masterAtomGroup::getBox(vector<float>&box) {
  trj->getBox(box);
}

AtomGroup::ptr masterAtomGroup::select_types(vector<string> &selTypes) {
  vector<string> all_types;
  vector<int> indices;
  topo->getTypes(all_types);
  for (const auto &selT : selTypes) {
    for (unsigned int atomNo=0;atomNo<all_types.size();atomNo++) {
      if (selT == all_types[atomNo]) {
        indices.push_back(atomNo);
      }
    }
  }

  // the indices need to be sorted to be useful
  sort(indices.begin(),indices.end());

  // there is a chance duplicate indices will be introduced, so this
  // will remove them
  indices.erase(unique(indices.begin(),indices.end()),indices.end());

  return AtomGroup::ptr(new subAtomGroup(shared_from_this(),indices));
}
