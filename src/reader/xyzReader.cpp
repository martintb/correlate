#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <map>

#include "Reader.hpp"
#include "xyzReader.hpp"
#include "debug.hpp"

using namespace std;

xyzReader::xyzReader(string fileName) : 
  Reader("xyz",
          Reader::POSITION | \
          Reader::BOX
        )
{
  this->fileExists(fileName);
  this->fileName = fileName;
  file.open(fileName,ios::in);
  frameStart = 0;
  frameSkip = 0;
  frameEnd = 0;
  numFrames = 1;

  if (!file) {
    cerr << "Error! Something is wrong with the lmpdump!" << endl;
    cerr << "good: "  << boolalpha << file.good() << endl;
    cerr << "eof:  "  << boolalpha << file.eof() << endl;
    cerr << "fail: "  << boolalpha << file.fail() << endl;
    cerr << "bad:  "  << boolalpha << file.bad() << endl;
    LOC();
    exit(EXIT_FAILURE);
  }


  box.resize(3);
  file >> natoms;
  file >> box[0] >> box[1] >> box[2];

  float dummy;
  x.resize(natoms);
  y.resize(natoms);
  z.resize(natoms);
  for (int i=0;i<natoms;++i) {
    file >> dummy >> x[i] >> y[i] >> z[i];
  }
  
  cout << "--> Success! Read xyz file." << endl;
}

xyzReader::~xyzReader() {
  file.close();
}


void xyzReader::readFrame(int frame) {
  cout << "==> ** Warning! Only one frame in XYZ file! Ignoring frame request **" << endl;
}

void xyzReader::printFileInfo() {
  cout << "File Name:          " << fileName << endl;
  cout << "Number of Frames:   " << numFrames << endl;
  cout << "First Frame Number: " << frameStart << endl;
  cout << "Last Frame Number:  " << frameEnd << endl;
  cout << "Frame Interval:     " << frameSkip << endl;
  cout << "Number of Atoms:    " << natoms << endl;
}

void xyzReader::getPositions(std::vector<float>&x,std::vector<float>&y,std::vector<float>&z)
{
  x = this->x;
  y = this->y;
  z = this->z;
}
void xyzReader::getBox(std::vector<float>& box)
{
  box = this->box;
}
