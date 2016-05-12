#include <memory>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

#include "inFile.hpp"
#include "debug.hpp"

using namespace std;

inFile::inFile(const string &basePath,const string &fileName) : File(basePath,fileName)
{
  if (not this->exists()) {
    cerr << "Error! inFile doesn't appear to exist." << endl;
    cerr << "inFile: " << path << endl;
    LOC();
    exit(EXIT_FAILURE);
  }
}

inFile::inFile(const string &fullPath) : File(fullPath) 
{
  if (not this->exists()) {
    cerr << "Error! inFile doesn't appear to exist." << endl;
    cerr << "inFile: " << path << endl;
    LOC();
    exit(EXIT_FAILURE);
  }
}
