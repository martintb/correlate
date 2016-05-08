#ifndef READER_HPP
#define READER_HPP

#include <string>
#include <map>
#include <memory>
#include <vector>

class Reader {
  public: 
    typedef std::shared_ptr<Reader> ptr;

    // to be implemented in cpp
    std::string const name; // name of subclasses
    void printReaderInfo();
    void fileExists(std::string);
    std::vector<std::string> splitStr(std::string in,std::string delim);

    static ptr get(std::string);
    static void printReaderList();

    // virtuals
    virtual void readFrame(int)=0;
    virtual void printFileInfo()=0;

    //Virtual File getters
    // If reader doesn't support this datatype
    // just define an empty function call. The caller
    // is required to check if its reference has be changed
    virtual void getPositions(std::vector<float>&,std::vector<float>&,std::vector<float>&)=0;
    virtual void getVelocities(std::vector<float>&,std::vector<float>&,std::vector<float>&)=0;
    virtual void getImages(std::vector<int>&,std::vector<int>&,std::vector<int>&)=0;
    virtual void getTypes(std::vector<std::string>&)=0;
    virtual void getMolecules(std::vector<int>&)=0;
    virtual void getBox(std::vector<float>&)=0;
    virtual void getCharge(std::vector<float>&)=0;

    // Variables to be used by subclasses
    int natoms;
    int numFrames;
    int frame;

    int const readsFlag; //bitflags that describes what a reader reads
    enum readsValues {
      NOTHING  = 0,
      POSITION = 1 << 0,
      VELOCITY = 1 << 1,
      IMAGE    = 1 << 2,
      BOX      = 1 << 3,
      TYPE     = 1 << 4,
      MOLECULE = 1 << 5,
      CHARGE   = 1 << 6,
    };

    static std::map<std::string,std::string> extInfo;


  protected:
    Reader(std::string nv,int fv) : name(nv),readsFlag(fv)  {};


};

#endif
