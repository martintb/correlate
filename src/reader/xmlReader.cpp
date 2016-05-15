#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "Reader.hpp"
#include "xmlReader.hpp"
#include "debug.hpp"

using namespace std;
namespace bpt = boost::property_tree;

xmlReader::xmlReader(string fileName) : 
  Reader("xml",
                 Reader::MOLECULE | \
                 Reader::TYPE    
               )
{
  this->fileExists(fileName);
  this->fileName = fileName;

  bpt::ptree tree;
  bpt::read_xml(fileName,tree);

  string typeStr = tree.get<std::string>("hoomd_xml.configuration.type");
  type = splitStr(typeStr,"\n");
  natoms = type.size();

  try {
    cout << "==> Found optional molecule section in xml file!" << endl;
    string molStr = tree.get<std::string>("hoomd_xml.configuration.molecule");
    vector<string> molVecStr = splitStr(molStr,"\n");
    std::transform(molVecStr.begin(), molVecStr.end(), std::back_inserter(molecule),
                   [](const std::string& str) { return std::stoi(str); });
  } catch( bpt::ptree_bad_path &e) {
    cout << "==> No (optional) molecule section found in xml. Adding all beads to the same molecule." << endl;
    molecule.assign(natoms,1);
  } 

  if (type.size() != molecule.size()) {
    cerr << "Error! Number of type and molecule entries in xml file don't match."<< endl;
    cerr << "type size: " << type.size() << endl;
    cerr << "molecule size: " << molecule.size() << endl;
    exit(EXIT_FAILURE);
  }
}

void xmlReader::readFrame(int) {
  cerr << "Warning! xml file only has a single frame of data. No other frames to read!" << endl;
}

void xmlReader::printFileInfo() {
  cout << "File Name:          " << fileName << endl;
  cout << "Number of Frames:   " << numFrames<< endl;
  cout << "Number of Atoms:    " << natoms   << endl;
}


void xmlReader::getTypes(std::vector<std::string >&type) {
  type = this->type;
}

void xmlReader::getMolecules(std::vector<int>&molecule) {
  molecule = this->molecule;
}
