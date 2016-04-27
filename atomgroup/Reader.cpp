#include <iostream>
#include <bitset>
#include <string>
#include <boost/filesystem.hpp>

#include "debug.hpp"
#include "Reader.hpp"

void Reader::contains() {
  std::cout << "READER TYPE: " << type                                        << std::endl;
  std::cout << "FLAGS:       " << static_cast<std::bitset< 8> >(flags)        << std::endl;
  std::cout << "NOTHING:     " << static_cast<bool>(flags & Reader::NOTHING)  << std::endl;
  std::cout << "POSITION:    " << static_cast<bool>(flags & Reader::POSITION) << std::endl;
  std::cout << "VELOCITY:    " << static_cast<bool>(flags & Reader::VELOCITY) << std::endl;
  std::cout << "IMAGE:       " << static_cast<bool>(flags & Reader::IMAGE)    << std::endl;
  std::cout << "BOX:         " << static_cast<bool>(flags & Reader::BOX)      << std::endl;
  std::cout << "TYPE:        " << static_cast<bool>(flags & Reader::TYPE)     << std::endl;
}

void Reader::exists(std::string p) {
  if (!boost::filesystem::exists(p)){
    std::cerr << "Error! The path you supplied to the reader doesn't appear to exist." << std::endl;
    std::cerr << "Path: " << p  << std::endl;
    LOC();
    exit(EXIT_FAILURE);
  }
}
