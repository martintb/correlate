#include <map>
#include <regex>
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>

#include "Reader.hpp"
#include "lmpFullReader.hpp"
#include "debug.hpp"

using namespace std;

lmpFullReader::lmpFullReader(string fileName) : 
  lmpBaseReader("lmpfull",
                 Reader::POSITION | \
                 Reader::IMAGE    | \
                 Reader::MOLECULE | \
                 Reader::BOX      | \
                 Reader::CHARGE   | \
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
  charge.resize(natoms);

  goToSection(f,"Atoms");
  float id;
  for (int i=0;i<natoms;i++) {
    f >> id >> molecule[i] >> type[i] >> charge[i] >> x[i] >> y[i] >> z[i] >> ix[i] >> iy[i] >> iz[i];
  }
  f.close();
}

void lmpFullReader::readFrame(int) {
  cerr << "Warning! lmpFull file only has a single frame of data. No other frames to read!" << endl;
}

void lmpFullReader::printFileInfo() {
  cout << "File Name:          " << fileName << endl;
  cout << "Number of Frames:   " << 1 << endl;
}

void lmpFullReader::getPositions(std::vector<float>&x,std::vector<float>&y,std::vector<float>&z) {
  x = this->x;
  y = this->y;
  z = this->z;
}
void lmpFullReader::getImages(std::vector<int>&ix,std::vector<int>&iy,std::vector<int>&iz) {
  ix = this->ix;
  iy = this->iy;
  iz = this->iz;
}
void lmpFullReader::getTypes(std::vector<std::string >&type) {
  type = this->type;
}
void lmpFullReader::getMolecules(std::vector<int>&molecule) {
  molecule = this->molecule;
}
void lmpFullReader::getBox(std::vector<float>& box) {
  box = this->box;
}
void lmpFullReader::getCharge(std::vector<float>& charge) {
  charge = this->charge;
}
