#ifndef DCDREADER_HPP
#define DCDREADER_HPP

#include <fstream>
#include <string>
#include <vector>
#include <memory>

#include "Reader.hpp"

class dcdReader: public Reader  {

  public:
    typedef std::shared_ptr<dcdReader> ptr;
    std::string fileName;
    dcdReader(std::string);
    ~dcdReader();

    std::vector<float> x;
    std::vector<float> y;
    std::vector<float> z;
    std::vector<float> box;

    float dt;
    int headerBytes; // bytes
    int frameBytes; // bytes
    int frameStart;
    int frameEnd;
    int frameSkip;
    char remarks[80];
    char timestamp[80];

    //Virtual File getters
    // If reader doesn't support this datatype
    // just define an empty function call. The caller
    // is required to check if its reference has be changed
    virtual void readFrame(int);
    virtual void printFileInfo();
    virtual void getPositions(std::vector<float>&,std::vector<float>&,std::vector<float>&);
    virtual void getBox(std::vector<float>&);

    //Virtuals not used by this class
    virtual void getVelocities(std::vector<float>&,std::vector<float>&,std::vector<float>&) {};
    virtual void getImages(std::vector<int>&,std::vector<int>&,std::vector<int>&) {};
    virtual void getTypes(std::vector<std::string>&) {};
    virtual void getMolecules(std::vector<int>&) {};


  private:
    void readHeader();
    void goToFrame(int);
    std::ifstream file;
    bool init;



};
#endif
