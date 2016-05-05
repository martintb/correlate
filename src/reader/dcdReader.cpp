#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include <cstdint>

#include "dcdReader.hpp"
#include "debug.hpp"

using namespace std;

dcdReader::dcdReader(string fileName) : 
  Reader("dcd",
          Reader::POSITION | \
          Reader::BOX
        )
{
  this->fileExists(fileName);
  this->fileName = fileName;
  file.open(fileName,ios::binary | ios::in);
  this->readHeader();
}

dcdReader::~dcdReader() {
  file.close();
}

void dcdReader::readHeader() {
  /* 
   * This was written based on the LAMMPS DCD writer and may not
   * work with DCD files from other codes
   */

  uint32_t CHECK;
  uint32_t dummy[13];
  char CORD[4];

  file.read((char*)&CHECK,sizeof(CHECK));
  file.read((char*)&CORD,sizeof(CORD));
  if (!(CHECK==84 && strncmp(CORD,"CORD",4)==0)) {
    cerr << "DCD Error!Something is wrong with the DCD header:" << endl;
    cerr << "CHECK (should be 84): " << (int) CHECK  << endl;
    cerr << "CORD (should be \"CORD\"): " << CORD << endl;
    LOC();
    exit(EXIT_FAILURE);
  }

  file.read((char*)&dummy,9*sizeof(uint32_t));
  numFrames = dummy[0];
  frameStart = dummy[1];
  frameSkip = dummy[2];
  frameEnd = dummy[3];

  file.read((char*)&dt,sizeof(float));
  file.read((char*)&dummy,13*sizeof(uint32_t));
  file.read((char*)&remarks,sizeof(remarks));
  file.read((char*)&timestamp,sizeof(timestamp));
  file.read((char*)&dummy,4*sizeof(uint32_t));
  natoms = dummy[2];

  headerBytes = file.tellg();
  frameBytes= 8*sizeof(uint32_t) + 3*natoms*sizeof(float) + 6*sizeof(double);

  x.resize(natoms);
  y.resize(natoms);
  z.resize(natoms);
  box.resize(3);
  readFrame(0);
}

void dcdReader::goToFrame(int frame=0) {
  if (frame<0 || frame>(numFrames-1)) {
    cerr << "Error! Requested frame number is outside of range:" << endl;
    cerr << "First Frame Number:      " << frameStart << endl;
    cerr << "Last Frame Number:       " << frameEnd << endl;
    cerr << "Requested Frame Number:  " << frame << endl;
    exit(EXIT_FAILURE);
  }
  file.seekg((frame*frameBytes+headerBytes));
  this->frame = frame;
}

void dcdReader::readFrame(int frame=0) {
  uint32_t dummy[2];
  double BOX[6];
  float X[natoms];
  float Y[natoms];
  float Z[natoms];

  this->goToFrame(frame);

  file.read((char*)&dummy,1*sizeof(uint32_t));
  file.read((char*)&BOX,sizeof(BOX));
  file.read((char*)&dummy,2*sizeof(uint32_t));
  file.read((char*)&X,sizeof(X));
  file.read((char*)&dummy,2*sizeof(uint32_t));
  file.read((char*)&Y,sizeof(Y));
  file.read((char*)&dummy,2*sizeof(uint32_t));
  file.read((char*)&Z,sizeof(Z));
  file.read((char*)&dummy,1*sizeof(uint32_t));

  box[0] = BOX[0];
  box[1] = BOX[2];
  box[2] = BOX[5];
  for (int i=0;i<natoms;i++) {
    x[i] = X[i];
    y[i] = Y[i];
    z[i] = Z[i];
  }

  if (frame==0) {
    int measured=((int) file.tellg())-headerBytes;
    if (frameBytes!=measured) {
      cerr << "Error! Frames are not sized according to expectations." << endl;
      cerr << "Estimated Frame Size: " << frameBytes << endl;
      cerr << "Measured Frame Size: " << measured << endl;
      cerr << "Frame sizes is estimated using:" << endl;
      cerr << "8*sizeof(uint32_t) + 3*natoms*sizeof(float) + 6*sizeof(double)" << endl;
      exit(EXIT_FAILURE);
    }
  }
}

void dcdReader::printFileInfo() {
  cout << "File Name:          " << fileName << endl;
  cout << "Number of Frames:   " << numFrames << endl;
  cout << "First Frame Number: " << frameStart << endl;
  cout << "Last Frame Number:  " << frameEnd << endl;
  cout << "Frame Interval:     " << frameSkip << endl;
  cout << "Number of Atoms:    " << natoms << endl;
  cout << "Time Step:          " << dt << endl;
  cout << "Remarks:            " << remarks << endl;
  cout << "Timestamp:          " << timestamp << endl;
  cout << "headerBytes:        " << headerBytes << endl;
  cout << "FrameBytes:         " << frameBytes << endl;
}

void dcdReader::getPositions(std::vector<float>&x,std::vector<float>&y,std::vector<float>&z)
{
  x = this->x;
  y = this->y;
  z = this->z;
}
void dcdReader::getBox(std::vector<float>& box)
{
  box = this->box;
}
