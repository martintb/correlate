#ifndef ATOMGROUP_HPP
#define ATOMGROUP_HPP
#include <vector>
#include <string>

#include "Reader.hpp"

class AtomGroup
{
  public:
    typedef std::shared_ptr<AtomGroup> ptr;
    static ptr make(std::string,std::string);

    int natoms;
    int numFrames;
    int frame;
    Reader::ptr topo;
    Reader::ptr trj;

    // Similar virtuals to Reader class
    virtual ptr select_types(std::vector<std::string> &selTypes)=0;
    virtual void readFrame(int)=0;
    virtual void getPositions(std::vector<float>&,std::vector<float>&,std::vector<float>&)=0;
    virtual void getTypes(std::vector<std::string>&)=0;
    virtual void getMolecules(std::vector<int>&)=0;
    virtual void getBox(std::vector<float>&)=0;

  protected:
    AtomGroup() {};
};
#endif
