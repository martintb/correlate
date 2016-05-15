#include <iostream>
#include <iomanip>
#include <map>
#include <bitset>
#include <string>
#include <cstring>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

#include "debug.hpp"
#include "Reader.hpp"
#include "ReaderList.hpp"

using namespace std;


void Reader::printReaderInfo() {
  cout << "Extension: " << name << endl;
  cout << "BitFlags:  " << static_cast< bitset<8> >(readsFlag)     << endl;
  cout << "Nothing:   "<<boolalpha<<static_cast<bool>(readsFlag & Reader::NOTHING)  << endl;
  cout << "Position:  "<<boolalpha<<static_cast<bool>(readsFlag & Reader::POSITION) << endl;
  cout << "Velocity:  "<<boolalpha<<static_cast<bool>(readsFlag & Reader::VELOCITY) << endl;
  cout << "Image:     "<<boolalpha<<static_cast<bool>(readsFlag & Reader::IMAGE)    << endl;
  cout << "Box:       "<<boolalpha<<static_cast<bool>(readsFlag & Reader::BOX)      << endl;
  cout << "Type:      "<<boolalpha<<static_cast<bool>(readsFlag & Reader::TYPE)     << endl;
  cout << "Molecule:  "<<boolalpha<<static_cast<bool>(readsFlag & Reader::MOLECULE) << endl;
  cout << "Charge:    "<<boolalpha<<static_cast<bool>(readsFlag & Reader::CHARGE)   << endl;
}

void Reader::fileExists(string p) {
  if (!boost::filesystem::exists(p)){
    cerr << "Error! The path you supplied to the reader doesn't appear to exist." << endl;
    cerr << "Path: " << p  << endl;
    LOC();
    exit(EXIT_FAILURE);
  }
}

vector<string> Reader::splitStr(string in,string delim) {
  vector<string> out;
  boost::trim(in);
  boost::split(out,in,boost::is_any_of(delim));
  return out;
}

map<string,string> Reader::extInfo = { 
                                        {".lmpbond","LAMMPS topology file w/ AtomStyle Bond"},
                                        {".lmpfull","LAMMPS topology file w/ AtomStyle Full"},
                                        {".lmpmolecular","LAMMPS topology file w/ AtomStyle Molecular"},
                                        {".dcd","DCD trajectory file"},
                                        {".topo","Simple 2-Column (Type,Molecule) Topology File"},
#ifdef XML
                                        {".xml","HOOMD-Blue based XML topology file"},
#endif 

                                     };

void Reader::printReaderList() {
  cout << "The following readers are available: " << endl;
  for (const auto &i : Reader::extInfo) {
    cout << std::left << std::setw(15) << i.first << " " << i.second << endl;
  }
}

Reader::ptr Reader::get(string fileName) {
  boost::filesystem::path p(fileName);

  if (p.extension().compare(".dcd")==0) {
    return make_shared<dcdReader>(fileName);

  } else if (p.extension().compare(".topo")==0) {
    return make_shared<topoReader>(fileName);

  } else if (p.extension().compare(".lmpbond")==0) {
    return make_shared<lmpBondReader>(fileName);

  } else if (p.extension().compare(".lmpfull")==0) {
    return make_shared<lmpFullReader>(fileName);

  } else if (p.extension().compare(".lmpmolecular")==0) {
    return make_shared<lmpMolecularReader>(fileName);

  } else {
    cerr << "Error! File extension not recognized!" << endl;
    cerr << "File extension: " << p.extension() << endl;
    Reader::printReaderList();
    LOC();
    exit(EXIT_FAILURE);
  }
}
