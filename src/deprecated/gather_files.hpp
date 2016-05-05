#ifndef GATHER_FILES_HPP
#define GATHER_FILES_HPP

#include <string>
#include <iterator>
#include <vector>
#include <boost/filesystem.hpp>

typedef std::vector<boost::filesystem::path> pathvec;
pathvec gather_files(std::string, std::string);

#endif
