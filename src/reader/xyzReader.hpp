#ifndef XYZ_READER_HPP
#define XYZ_READER_HPP

#include <fstream>
#include <string>
#include <vector>
#include <memory>

#include "NotProvidedException.hpp"
#include "Reader.hpp"

class xyzReader: public Reader  {

  public:
    typedef std::shared_ptr<xyzReader> ptr;
    std::string fileName;
    xyzReader(std::string);
    ~xyzReader();

    std::vector<float> x;
    std::vector<float> y;
    std::vector<float> z;
    std::vector<float> box;

    long frameStart;
    long frameEnd;
    long frameSkip;

    //Virtual File getters
    // If reader doesn't support this datatype
    // just define an empty function call. The caller
    // is required to check if its reference has be changed
    virtual void readFrame(int);
    virtual void printFileInfo();
    virtual void getPositions(std::vector<float>&,std::vector<float>&,std::vector<float>&);
    virtual void getBox(std::vector<float>&);

    //Virtuals not used by this class
    virtual void getVelocities(std::vector<float>&,std::vector<float>&,std::vector<float>&) {
      throw NotProvidedException("xyzReader","velocities");
    };
    virtual void getImages(std::vector<int>&,std::vector<int>&,std::vector<int>&) {
      throw NotProvidedException("xyzReader","images");
    };
    virtual void getTypes(std::vector<std::string>&) {
      throw NotProvidedException("xyzReader","types");
    };
    virtual void getMolecules(std::vector<int>&) {
      throw NotProvidedException("xyzReader","molecule numbers");
    };
    virtual void getCharge(std::vector<float>&) {
      throw NotProvidedException("xyzReader","charges");
    };


  private:
    std::ifstream file;



};
#endif
