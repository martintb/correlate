#ifndef LMPMOLECULAR_READER_HPP
#define LMPMOLECULAR_READER_HPP

#include <fstream>
#include <string>
#include <vector>
#include <memory>

#include "lmpBaseReader.hpp"

class lmpMolecularReader: public lmpBaseReader {

  public:
    typedef std::shared_ptr<lmpMolecularReader> ptr;
    std::string fileName;
    lmpMolecularReader(std::string);
    ~lmpMolecularReader() {};

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
    virtual void getCharge(std::vector<float>&) {};
    virtual void getVelocities(std::vector<float>&,std::vector<float>&,std::vector<float>&) {};

  private:


};
#endif
