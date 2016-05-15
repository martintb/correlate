#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>

#include "Reader.hpp"
#include "topoReader.hpp"
#include "debug.hpp"

using namespace std;

topoReader::topoReader(string fileName) : 
  Reader("topo",
                 Reader::MOLECULE | \
                 Reader::TYPE    
               )
{
  this->fileExists(fileName);
  this->fileName = fileName;
  numFrames = 1;

  ifstream f(fileName);
  natoms = 0;
  while (true) {
    string typeVal;
    float molVal;
    f >> typeVal >> molVal;
    if (f.eof()) break;
    type.push_back(typeVal);
    molecule.push_back(molVal);
    natoms+=1;
  }
  f.close();
}

void topoReader::readFrame(int) {
  cerr << "Warning! topo file only has a single frame of data. No other frames to read!" << endl;
}

void topoReader::printFileInfo() {
  cout << "File Name:          " << fileName << endl;
  cout << "Number of Frames:   " << numFrames<< endl;
  cout << "Number of Atoms:    " << natoms   << endl;
}

void topoReader::getTypes(std::vector<std::string >&type) {
  type = this->type;
}
void topoReader::getMolecules(std::vector<int>&molecule) {
  molecule = this->molecule;
}
