#ifndef OUTFILE_HPP
#define OUTFILE_HPP

#include <memory>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

#include "File.hpp"

class outFile : public File
{
  public:
    outFile(const std::string&,const std::string&, bool overwrite);
    outFile(const std::string&, bool overwrit);
    ~outFile() {};

    bool overwrite;
};

#endif
