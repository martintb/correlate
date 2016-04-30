#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iterator>
#include <vector>
#include <algorithm>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

#include "gather_files.hpp"
#include "debug.hpp"

using namespace std;

pathvec gather_files(string path_str, string regex_str)
{
  boost::filesystem::path p (path_str);           // p reads clearer than argv[1] in the following code
  boost::regex path_filter(regex_str);            // p reads clearer than argv[1] in the following code

  pathvec v2;                                     // so we can sort them later
  try {
    if (boost::filesystem::exists(p))   {         // does p actually exist?  
      if (boost::filesystem::is_directory(p)) {   // is p a directory? 
        pathvec v1;                               // so we can sort them later

        copy(boost::filesystem::directory_iterator(p), 
             boost::filesystem::directory_iterator(), 
             back_inserter(v1));

        sort(v1.begin(), v1.end());             // sort, since directory iteration
                                              // is not ordered on some file systems

        boost::smatch what;
        for (pathvec::const_iterator it(v1.begin()), it_end(v1.end()); it != it_end; ++it) {
          if( !boost::regex_match( it->filename().string(), what, path_filter ) ) continue;
          v2.push_back(*it);
        }
      } else {
        cerr << "Error! Problem with supplied path." << endl;
        cerr << "Not a directory: " << p << endl;
      }
    } else {
      cerr << "Error! Problem with supplied path." << endl;
      cerr << "Does not exist: " << p << endl;
    }
  } catch (const boost::filesystem::filesystem_error& ex) {
    cerr << ex.what() << endl;
    LOC();
    exit(EXIT_FAILURE);
  }

  return v2;
};
