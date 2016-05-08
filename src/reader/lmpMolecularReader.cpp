#include <map>
#include <regex>
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>

#include "Reader.hpp"
#include "lmpMolecularReader.hpp"
#include "debug.hpp"

using namespace std;

lmpMolecularReader::lmpMolecularReader(string fileName) : 
  lmpBaseReader("lmpmolecular",
                 Reader::POSITION | \
                 Reader::IMAGE    | \
                 Reader::MOLECULE | \
                 Reader::BOX      | \
                 Reader::TYPE    
                 
               )
{
  this->fileExists(fileName);
  this->fileName = fileName;
  ifstream f(fileName);

  auto headerData = readHeader(f);
  natoms = headerData["natoms"];

  box.resize(3);
  box[0] = headerData["lx"];
  box[1] = headerData["ly"];
  box[2] = headerData["lz"];

  type.resize(natoms);
  molecule.resize(natoms);
  ix.resize(natoms);
  iy.resize(natoms);
  iz.resize(natoms);
  x.resize(natoms);
  y.resize(natoms);
  z.resize(natoms);

  goToSection(f,"Atoms");
  string line;
  getline(f,line);
  auto splitLine = splitStr(line," ");
  int ncols = splitLine.size();
  if (ncols == AtomCols1) {
    cout << "--> Reading AtomStyle Molecular w/ image data" << endl;
  } else if (ncols==AtomCols2) {
    cout << "--> Reading AtomStyle Molecular w/out image data" << endl;
  } else {
    cerr << "Error! There are two options for the number of cols in AtomStyle Molecular data file:" << endl;
    cerr << "with image flags: " << AtomCols1 << endl;
    cerr << "no   image flags: " << AtomCols2 << endl;
    cerr << "number of columns found: " << ncols << endl;
    cerr << "Line: " << line << endl;
    LOC();
    exit(EXIT_FAILURE);
  }

  goToSection(f,"Atoms");
  float id;
  for (int i=0;i<natoms;i++) {
    if (ncols == AtomCols1) {
      f >> id >> molecule[i] >> type[i] >> x[i] >> y[i] >> z[i] >> ix[i] >> iy[i] >> iz[i];
    } else if (ncols==AtomCols2) {
      f >> id >> molecule[i] >> type[i] >> x[i] >> y[i] >> z[i];
    }
  }
  f.close();
}

void lmpMolecularReader::readFrame(int) {
  cerr << "Warning! lmpMolecular file only has a single frame of data. No other frames to read!" << endl;
}

void lmpMolecularReader::printFileInfo() {
  cout << "File Name:          " << fileName << endl;
  cout << "Number of Frames:   " << 1 << endl;
}

void lmpMolecularReader::getPositions(std::vector<float>&x,std::vector<float>&y,std::vector<float>&z) {
  x = this->x;
  y = this->y;
  z = this->z;
}
void lmpMolecularReader::getImages(std::vector<int>&ix,std::vector<int>&iy,std::vector<int>&iz) {
  ix = this->ix;
  iy = this->iy;
  iz = this->iz;
}
void lmpMolecularReader::getTypes(std::vector<std::string >&type) {
  type = this->type;
}
void lmpMolecularReader::getMolecules(std::vector<int>&molecule) {
  molecule = this->molecule;
}
void lmpMolecularReader::getBox(std::vector<float>& box) {
  box = this->box;
}
