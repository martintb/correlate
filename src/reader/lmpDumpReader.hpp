#ifndef LMPDUMP_READER_HPP
#define LMPDUMP_READER_HPP

#include <fstream>
#include <string>
#include <vector>
#include <memory>

#include "NotProvidedException.hpp"
#include "Reader.hpp"

class lmpDumpReader: public Reader  {

  public:
    typedef std::shared_ptr<lmpDumpReader> ptr;
    std::string fileName;
    lmpDumpReader(std::string);
    ~lmpDumpReader();

    std::vector<float> x;
    std::vector<float> y;
    std::vector<float> z;
    std::vector<float> box;

    std::vector<int> frameMap;
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
      throw NotProvidedException("lmpDumpReader","velocities");
    };
    virtual void getImages(std::vector<int>&,std::vector<int>&,std::vector<int>&) {
      throw NotProvidedException("lmpDumpReader","images");
    };
    virtual void getTypes(std::vector<std::string>&) {
      throw NotProvidedException("lmpDumpReader","types");
    };
    virtual void getMolecules(std::vector<int>&) {
      throw NotProvidedException("lmpDumpReader","molecule numbers");
    };
    virtual void getCharge(std::vector<float>&) {
      throw NotProvidedException("lmpDumpReader","charges");
    };


  private:
    void initialParse();
    std::ifstream file;
    std::string nextItem();
    std::string nextItem(std::string);
    void goToFrame(int frame, bool recurse=false);



};
#endif
