#include <iostream>
#include <iomanip>
#include <sstream>
#include <memory>
#include <string>
#include <iterator>
#include <vector>
#include <algorithm>
#include <boost/filesystem.hpp>
#include <regex>

#include "File.hpp"
#include "debug.hpp"

using namespace std;
namespace bfs = boost::filesystem;

File::File(const string &basePath, const string &fileName) {
  //contruct full path
  path  = basePath;
  path /= fileName;
}

File::File(const std::string &fullPath) {
  path = fullPath;
}

bool File::exists() {
  return bfs::exists(path);
}

bfs::path File::nextPath(bfs::path oldPath) {
  // gather all files in base directory
  string regex_str = oldPath.stem().string() + "\\.?([0-9]{4}).*";
  regex path_filter(regex_str);
  File::pathVec matched_paths = gatherPaths(oldPath.parent_path(),path_filter);

  // pull numbers from files and
  smatch m;
  int temp;
  vector<int> file_numbers;
  for (const auto &i : matched_paths) {
    regex_search(i.filename().string(),m,path_filter);
    istringstream (m[1].str()) >> temp;
    file_numbers.push_back(temp);
  }

  // find max file number
  int max_file_number=1;
  if (file_numbers.size()>0) {
    max_file_number = *max_element(file_numbers.begin(),file_numbers.end());
  }

  ostringstream os;
  string newFileName;
  newFileName += oldPath.stem().string() + ".";
  os << setw(4) << setfill('0') << ++max_file_number;
  newFileName += os.str()  + oldPath.extension().string();

  bfs::path newPath = oldPath.parent_path() / bfs::path(newFileName);
  return newPath;
}

File::pathVec File::gatherPaths(bfs::path path,regex path_filter) {
  // regex path_filter(regex_str);  
  File::pathVec matched_files;
  try {
    if (bfs::exists(path))   {       
      if (bfs::is_directory(path)) {
        pathVec allFiles; 
        copy(bfs::directory_iterator(path), 
             bfs::directory_iterator(), 
             back_inserter(allFiles));
        sort(allFiles.begin(), allFiles.end());  
               
        smatch what;
        for (const auto &it : allFiles) {
          if( !regex_match( it.filename().string(), what, path_filter ) ) continue;
          matched_files.push_back(it);
        }
      } else {
        cerr << "Error! Problem with supplied path." << endl;
        cerr << "Not a directory: " << path << endl;
      }
    } else {
      cerr << "Error! Problem with supplied path." << endl;
      cerr << "Does not exist: " << path << endl;
    }
  } catch (const bfs::filesystem_error& ex) {
    cerr << ex.what() << endl;
    LOC();
    exit(EXIT_FAILURE);
  }

  return matched_files;
}
