#ifndef READER_HPP
#define READER_HPP

#include <armadillo>
#include <string>
#include <memory>
#include <vector>

class Reader {
  public: 
    typedef std::shared_ptr<Reader> ptr;
    std::string const type;
    int const flags;
    void exists(std::string);
    void contains();

    // virtuals
    virtual void readFrame(int)=0;
    virtual void printFileInfo()=0;
    virtual arma::Mat<float> getPositions()=0;
    virtual double getLx()=0;
    virtual double getLy()=0;
    virtual double getLz()=0;
    // virtual std::vector<std::string> getTypes();


    enum FlagValues {
      NOTHING  = 0,
      POSITION = 1 << 0,
      VELOCITY = 1 << 1,
      IMAGE    = 1 << 2,
      BOX      = 1 << 3,
      TYPE     = 1 << 4,
      MOLECULE = 1 << 5,
    };


  protected:
    Reader(std::string tv,int fv): type(tv),flags(fv)  {};


};

#endif
