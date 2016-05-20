#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <map>

#include "Reader.hpp"
#include "lmpDumpReader.hpp"
#include "debug.hpp"

using namespace std;

lmpDumpReader::lmpDumpReader(string fileName) : 
  Reader("lmpdump",
          Reader::POSITION | \
          Reader::BOX
        )
{
  this->fileExists(fileName);
  this->fileName = fileName;
  file.open(fileName,ios::in);
  this->initialParse();
  readFrame(0);
}

lmpDumpReader::~lmpDumpReader() {
  file.close();
}

std::string lmpDumpReader::nextItem(string tag)
{
  if (file.eof()) return " ";
  regex sectionRE("^ITEM: "+tag+".*",regex::extended);
  string line;
  while (getline(file,line)) {
    if (regex_search(line,sectionRE)) {
      break;
    }
  }
  return line;
}

std::string lmpDumpReader::nextItem()
{
  if (file.eof()) return " ";
  string line;
  while (getline(file,line)) {
    if (line.compare(0,4,"ITEM")) {
      break;
    }
  }
  return line;
}

void lmpDumpReader::initialParse() {
  cout << "--> Doing an initial read of the lmpdump..." << endl;
  file.seekg(0,ios::beg); //return to top of file

  int timestep;
  string item;
  frameMap.clear();
  while (true) {
    item = this->nextItem("TIMESTEP");
    if (file.eof()) break;
    file >> timestep;
    // skip repeated frames
    if ((frameMap.size()>0) and (frameMap.back()==timestep)){
      continue;
    }
    frameMap.push_back(timestep);
  }

  frameStart = frameMap.front();
  frameEnd = frameMap.back();
  if (frameMap.size()>0) {
    frameSkip = frameMap[1] - frameMap[0];
  } else {
    frameSkip = 1;
  }
  numFrames = frameMap.size();
  file.clear();
  file.seekg(0,ios::beg); //return to top of file
  cout << "--> Success! Read lmpdump and created timestep->framenumber map." << endl;
}

void lmpDumpReader::goToFrame(int frame, bool recurse) {
  /* This function is designed so that it'll search forward from
   * the current point in the file for the specified frame. If that 
   * frame is not found, it'll reset the pointer to the beginning of
   * the file and try again. This makes forward iteration through the 
   * file more efficient, which is the likely use case.
   */

  if ((frame>(numFrames-1)) or (frame<0)) {
    cerr << "Error! Frame is out of bounds for this lmpdump!" << endl;
    cerr << "Frame: " << frame << endl;
    LOC();
    exit(EXIT_FAILURE);
  }

  int target = frameMap[frame];
  int timestep;
  string item;
  while (true) {
    if (file.eof()) break;
    item = this->nextItem("TIMESTEP");
    file >> timestep;
    if (timestep == target) {
      this->frame = frame;
      return;
    }
  }

  // we didn't find the frame from the current position,
  // so we go back to the start and try again (but only once)
  if (recurse) {
    file.clear();
    file.seekg(0,ios::beg); //return to top of file
    goToFrame(frame,false);// we don't want infinite recursion
  }

  // if we've made it this far, something has gone terribly wrong
  cerr << "Error! Frame not found in file!" << endl;
  cerr << "Frame: " << frame << endl;
  cerr << "numFrames: " << numFrames << endl;
  exit(EXIT_FAILURE);
}

void lmpDumpReader::readFrame(int frame) {
  if (!file) {
    cerr << "Error! Something is wrong with the lmpdump!" << endl;
    cerr << "good: "  << boolalpha << file.good() << endl;
    cerr << "eof:  "  << boolalpha << file.eof() << endl;
    cerr << "fail: "  << boolalpha << file.fail() << endl;
    cerr << "bad:  "  << boolalpha << file.bad() << endl;
    LOC();
    exit(EXIT_FAILURE);
  }

  this->goToFrame(frame,/*recurse=*/true); 

  string item;
  item = this->nextItem("NUMBER OF ATOMS");
  file >> natoms;

  float xlo,xhi;
  float ylo,yhi;
  float zlo,zhi;
  box.resize(3);
  item = this->nextItem("BOX BOUNDS");
  file >> xlo >> xhi;
  file >> ylo >> yhi;
  file >> zlo >> zhi;
  box[0] = (xhi - xlo);
  box[1] = (yhi - ylo);
  box[2] = (zhi - zlo);

  item = this->nextItem("ATOMS");
  auto keys = splitStr(item," ");
  map<string,vector<float> > dataMap;
  float temp;
  for (int i=0;i<natoms;++i) {
    // the first two keys are ITEM: and ATOMS
    for (auto key = keys.begin()+2;key!=keys.end();++key) {
      file>>temp;
      dataMap[*key].push_back(temp);
    }
  }
  
  // lmpDump files can be unsorted, so this loop
  // effectively sorts the values by id
  x.resize(natoms);
  y.resize(natoms);
  z.resize(natoms);
  for (int i=0;i<natoms;i++) {
    int id = static_cast<int>(dataMap["id"][i])-1;
    x[id] = dataMap["x"][i];
    y[id] = dataMap["y"][i];
    z[id] = dataMap["z"][i];
  }
}

void lmpDumpReader::printFileInfo() {
  cout << "File Name:          " << fileName << endl;
  cout << "Number of Frames:   " << numFrames << endl;
  cout << "First Frame Number: " << frameStart << endl;
  cout << "Last Frame Number:  " << frameEnd << endl;
  cout << "Frame Interval:     " << frameSkip << endl;
  cout << "Number of Atoms:    " << natoms << endl;
}

void lmpDumpReader::getPositions(std::vector<float>&x,std::vector<float>&y,std::vector<float>&z)
{
  x = this->x;
  y = this->y;
  z = this->z;
}
void lmpDumpReader::getBox(std::vector<float>& box)
{
  box = this->box;
}
