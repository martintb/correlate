#ifndef DCD_HPP
#define DCD_HPP

#include <fstream>
#include <armadillo>
#include <string>
#include <vector>
#include <memory>

#include "Reader.hpp"

class DCD: public Reader  {

  public:
  // protected:
    std::string fileName;

    float dt;
    double lx;
    double ly;
    double lz;
    arma::Mat<float> pos;

    int frame;
    int headerBytes; // bytes
    int frameBytes; // bytes
    int numFrames;
    int frameStart;
    int frameEnd;
    int frameSkip;
    int natoms; 
    char remarks[80];
    char timestamp[80];

  // public:
    typedef std::shared_ptr<DCD> ptr;
    // const int flags=Reader::POSITION | Reader::BOX;

    DCD(std::string);
    ~DCD();

    // derived virtual functions
    void printFileInfo();
    void readFrame(int);
    arma::Mat<float> getPositions() {return pos;};
    double getLx() {return lx;};
    double getLy() {return ly;};
    double getLz() {return lz;};

    void readHeader();
    void goToFrame(int);
    std::ifstream file;


    bool init;

};
#endif
