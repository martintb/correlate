#include <iostream>
#include <memory>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

#include "outFile.hpp"
#include "debug.hpp"

using namespace std;
namespace bfs = boost::filesystem;

outFile::outFile(const string &basePath,const string &fileName,bool overwrite) : File(basePath,fileName)
{
  this->overwrite = overwrite;
  if ((not overwrite) and (this->exists())) {
    bfs::path newPath = nextPath(path);
    cout << "--> File Exists! Renaming output file: ";
    cout << path << " ==> " << newPath << endl;
    this->path = newPath;
  } 
}

outFile::outFile(const string &fullPath,bool overwrite) : File(fullPath) 
{
  this->overwrite = overwrite;
  if ((not overwrite) and (this->exists())) {
    bfs::path newPath = nextPath(path);
    cout << "--> File Exists! Renaming output file: ";
    cout << path << " ==> " << newPath << endl;
    this->path = newPath;
  }
}
