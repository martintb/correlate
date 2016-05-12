#ifndef FILE_HPP
#define FILE_HPP

#include <memory>
#include <string>
#include <regex>
#include <boost/filesystem.hpp>

class File
{
  public:
    typedef std::shared_ptr<File> ptr;
    typedef std::vector<boost::filesystem::path> pathVec;
    File(const std::string&,const std::string&);
    File(const std::string&);
    ~File() {};

    bool exists();
    boost::filesystem::path nextPath(boost::filesystem::path);
    pathVec gatherPaths(boost::filesystem::path,std::regex);
    boost::filesystem::path path; 
};

#endif
