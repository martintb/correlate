#ifndef LMPBASE_READER_HPP
#define LMPBASE_READER_HPP

// The lmpReaders make heavy use of the regex functionality 
// which was only *properly* implemented in gcc 4.9+. The fun part
// is that earlier versions of gcc will allow you to import "broken"
// versions of the regex library. Fun stuff. 
#ifdef __GNUG__
  #define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100  + __GNUC_PATCHLEVEL__)
  #if GCC_VERSION < 40900 //test for GCC>4.9 
    #error "GCC VERSION must be >4.9 to use lmpData Readers!"
  #endif
#endif

#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <map>

#include "Reader.hpp"

class lmpBaseReader: public Reader  {

  public:
    lmpBaseReader(std::string sv,int nv) : Reader(sv,nv) {}; //these functions should never be called
    ~lmpBaseReader() {}; //these functions should never be called

    void goToSection(std::ifstream&,std::string);
    std::map<std::string,float> readHeader(std::ifstream&);

    std::vector<std::string> type;
    std::vector<float> box;
    std::vector<float> charge;
    std::vector<int> molecule;
    std::vector<int> ix;
    std::vector<int> iy;
    std::vector<int> iz;
    std::vector<float> x;
    std::vector<float> y;
    std::vector<float> z;
    std::vector<float> vx;
    std::vector<float> vy;
    std::vector<float> vz;

    //Virtual File getters
    // If reader doesn't support this datatype
    // just define an empty function call. The caller
    // is required to check if its reference has be changed
    virtual void readFrame(int)=0;
    virtual void printFileInfo()=0;
    virtual void getPositions(std::vector<float>&,std::vector<float>&,std::vector<float>&)=0;
    virtual void getImages(std::vector<int>&,std::vector<int>&,std::vector<int>&)=0;
    virtual void getTypes(std::vector<std::string>&)=0;
    virtual void getMolecules(std::vector<int>&)=0;
    virtual void getBox(std::vector<float>&)=0;
    virtual void getVelocities(std::vector<float>&,std::vector<float>&,std::vector<float>&)=0;

  



};
#endif
