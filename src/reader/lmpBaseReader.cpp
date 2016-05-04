#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <map>

#include "lmpBaseReader.hpp"

using namespace std;

void lmpBaseReader::goToSection(ifstream &f, string section_title)
{
  f.seekg(0,ios::beg); //return to top of file
  regex sectionRE(section_title+".*",regex::extended);
  string line;
  while (getline(f,line)) {
    if (regex_search(line,sectionRE)) {
      getline(f,line);// blank lines
      break;
    }
  }
}

map<string,float> lmpBaseReader::readHeader(ifstream &f)
{
  f.seekg(0,ios::beg); //return to top of file

  regex natoms("(.*) atoms",regex::extended);
  regex nbonds("(.*) bonds",regex::extended);
  regex nangles("(.*) angles",regex::extended);
  regex boxx("(.*) (.*) xlo xhi",regex::extended);
  regex boxy("(.*) (.*) ylo yhi",regex::extended);
  regex boxz("(.*) (.*) zlo zhi",regex::extended);

  map<string,float>  headerData;

  smatch m;
  string line;
  float temp1,temp2;
  int lineNo=0;
  int maxHeaderSize = 50;
  while (getline(f,line)) {
    if (regex_search(line,m,boxz)) {
      istringstream (m[1].str()) >> temp1;
      istringstream (m[2].str()) >> temp2;
      headerData["zlo"] = temp1;
      headerData["zhi"] = temp2;
      headerData["lz"]  = temp2 - temp1;
    } else if (regex_search(line,m,boxy)) {
      istringstream (m[1].str()) >> temp1;
      istringstream (m[2].str()) >> temp2;
      headerData["ylo"] = temp1;
      headerData["yhi"] = temp2;
      headerData["ly"]  = temp2 - temp1;
    } else if (regex_search(line,m,boxx)) {
      istringstream (m[1].str()) >> temp1;
      istringstream (m[2].str()) >> temp2;
      headerData["xlo"] = temp1;
      headerData["xhi"] = temp2;
      headerData["lx"]  = temp2 - temp1;
    } else if (regex_search(line,m,nangles)) {
      istringstream (m[1].str()) >> temp1;
      headerData["nangles"] = temp1;
    } else if (regex_search(line,m,nbonds)) {
      istringstream (m[1].str()) >> temp1;
      headerData["nbonds"] = temp1;
    } else if (regex_search(line,m,natoms)) {
      istringstream (m[1].str()) >> temp1;
      headerData["natoms"] = temp1;
      cout << "FOUND IT " << temp1 << endl;
    }
    if (lineNo>maxHeaderSize) break;
    lineNo++;
  }
  return headerData;
}
