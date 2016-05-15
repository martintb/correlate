#ifndef TOPOREADER_HPP
#define TOPOREADER_HPP

#include <fstream>
#include <string>
#include <vector>
#include <memory>

#include "Reader.hpp"

class topoReader: public Reader  {

  public:
    typedef std::shared_ptr<topoReader> ptr;
    std::string fileName;
    topoReader(std::string);
    ~topoReader() {};

    std::vector<int> molecule;
    std::vector<std::string> type;

    //Virtual File getters
    // If reader doesn't support this datatype
    // just define an empty function call. The caller
    // is required to check if its reference has be changed
    virtual void readFrame(int);
    virtual void printFileInfo();
    virtual void getMolecules(std::vector<int>&);
    virtual void getTypes(std::vector<std::string>&);

    //Virtuals not used by this class
    virtual void getVelocities(std::vector<float>&,std::vector<float>&,std::vector<float>&) {};
    virtual void getImages(std::vector<int>&,std::vector<int>&,std::vector<int>&) {};
    virtual void getCharge(std::vector<float>&) {};
    virtual void getPositions(std::vector<float>&,std::vector<float>&,std::vector<float>&) {};
    virtual void getBox(std::vector<float>&) {};




};
#endif
