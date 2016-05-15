#ifndef XML_READER_HPP
#define XML_READER_HPP

#include <fstream>
#include <string>
#include <vector>
#include <memory>

#include "NotProvidedException.hpp"
#include "Reader.hpp"

class xmlReader: public Reader  {

  public:
    typedef std::shared_ptr<xmlReader> ptr;
    std::string fileName;
    xmlReader(std::string);
    ~xmlReader() {};

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
    virtual void getVelocities(std::vector<float>&,std::vector<float>&,std::vector<float>&) {
      throw NotProvidedException("xmlReader","velocities");
    };
    virtual void getImages(std::vector<int>&,std::vector<int>&,std::vector<int>&) {
      throw NotProvidedException("xmlReader","image flags");
    };
    virtual void getCharge(std::vector<float>&) {
      throw NotProvidedException("xmlReader","charges");
    };
    virtual void getPositions(std::vector<float>&,std::vector<float>&,std::vector<float>&) {
      throw NotProvidedException("xmlReader","positions");
    };
    virtual void getBox(std::vector<float>&) {
      throw NotProvidedException("xmlReader","box lengths");
    };




};
#endif
