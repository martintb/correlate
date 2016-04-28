#ifndef ATOMGROUP_HPP
#define ATOMGROUP_HPP

#include <armadillo>
#include <vector>
#include <string>
#include <memory>
#include "XML.hpp"
#include "DCD.hpp"

class AtomGroup {
  public:
    AtomGroup(std::string,std::string);
    ~AtomGroup();

    typedef std::shared_ptr<AtomGroup> ptr;

    void readFrame(int);
    void readFiles();
    void setSize(int);
    ptr select(const std::vector<std::string>&);
    void mask(ptr, arma::umat&);
    std::vector<float> toSTLVec(float dim);

    std::string dcdPath;
    std::string xmlPath;
    bool init;

    XML::ptr xptr;
    DCD::ptr dptr;

    double lx;
    double ly;
    double lz;
    arma::Mat<float> pos;
    std::vector<std::string> type;
    int natoms;
    int frame;

};
#endif
