#ifndef LMPMOLECULAR_READER_HPP
#define LMPMOLECULAR_READER_HPP

#include <fstream>
#include <string>
#include <vector>
#include <memory>

#include "NotProvidedException.hpp"
#include "lmpBaseReader.hpp"

class lmpMolecularReader: public lmpBaseReader {

  public:
    typedef std::shared_ptr<lmpMolecularReader> ptr;
    std::string fileName;
    lmpMolecularReader(std::string);
    ~lmpMolecularReader() {};

    const int AtomCols1 = 9; //with image data
    const int AtomCols2 = 6; //with-out image data

    //Virtual File getters
    // If reader doesn't support this datatype
    // just define an empty function call. The caller
    // is required to check if its reference has be changed
    virtual void readFrame(int);
    virtual void printFileInfo();
    virtual void getPositions(std::vector<float>&,std::vector<float>&,std::vector<float>&);
    virtual void getImages(std::vector<int>&,std::vector<int>&,std::vector<int>&);
    virtual void getTypes(std::vector<std::string>&);
    virtual void getMolecules(std::vector<int>&);
    virtual void getBox(std::vector<float>&);
    virtual void getCharge(std::vector<float>&) {
      throw NotProvidedException("lmpMolecularReader","charges");
    };
    virtual void getVelocities(std::vector<float>&,std::vector<float>&,std::vector<float>&) {
      throw NotProvidedException("lmpMolecularReader","velocities");
    };

  private:


};
#endif
