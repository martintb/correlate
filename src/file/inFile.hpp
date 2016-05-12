#ifndef INFILE_HPP
#define INFILE_HPP

#include <memory>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

#include "File.hpp"

class inFile : public File
{
  public:
    inFile(const std::string&,const std::string&);
    inFile(const std::string&);
    ~inFile() {};
};

#endif
