#include <iostream>
#include <iomanip>
#include <map>
#include <bitset>
#include <string>
#include <cstring>
#include <boost/filesystem.hpp>

#include "debug.hpp"
#include "Reader.hpp"
#include "ReaderList.hpp"

using namespace std;


void Reader::printReaderInfo() {
  cout << "Reader Name:" << name << endl;
  cout << "------------" << endl;
  cout << "BITFLAGS: " << static_cast< bitset<8> >(readsFlag)     << endl;
  cout << "NOTHING:  "<<boolalpha<<static_cast<bool>(readsFlag & Reader::NOTHING)  << endl;
  cout << "POSITION: "<<boolalpha<<static_cast<bool>(readsFlag & Reader::POSITION) << endl;
  cout << "VELOCITY: "<<boolalpha<<static_cast<bool>(readsFlag & Reader::VELOCITY) << endl;
  cout << "IMAGE:    "<<boolalpha<<static_cast<bool>(readsFlag & Reader::IMAGE)    << endl;
  cout << "BOX:      "<<boolalpha<<static_cast<bool>(readsFlag & Reader::BOX)      << endl;
  cout << "TYPE:     "<<boolalpha<<static_cast<bool>(readsFlag & Reader::TYPE)     << endl;
  cout << "MOLECULE: "<<boolalpha<<static_cast<bool>(readsFlag & Reader::MOLECULE) << endl;
  cout << "CHARGE:   "<<boolalpha<<static_cast<bool>(readsFlag & Reader::CHARGE)   << endl;
}

void Reader::fileExists(string p) {
  if (!boost::filesystem::exists(p)){
    cerr << "Error! The path you supplied to the reader doesn't appear to exist." << endl;
    cerr << "Path: " << p  << endl;
    LOC();
    exit(EXIT_FAILURE);
  }
}

map<string,string> Reader::extInfo = { 
                                        {".lammps","LAMMPS structure file"},
                                        {".lmptrj","LAMMPS trajectory file"},
                                        {".xml","HOOMD-Blue based structure file"},
                                        {".dcd","DCD trajectory file"},
                                     };

void Reader::printReaderList() {
  cout << "The following readers are available (hopefully): " << endl;
  for (auto &i : Reader::extInfo) {
    cout << setw(10) << i.first << " " << i.second << endl;
  }
}

Reader::ptr Reader::get(string fileName) {
  boost::filesystem::path p(fileName);
  if (p.extension().compare(".dcd")==0) {
    return Reader::ptr(new dcdReader(fileName));
  } else {
    cerr << "Error! File extension not recognized!" << endl;
    cerr << "File extension: " << p.extension() << endl;
    Reader::printReaderList();
    LOC();
    exit(EXIT_FAILURE);
  }
}
