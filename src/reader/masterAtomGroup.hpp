#ifndef MASTER_ATOMGROUP_HPP
#define MASTER_ATOMGROUP_HPP
#include <vector>
#include <string>

#include "AtomGroup.hpp"

class masterAtomGroup : public AtomGroup {
  public:
    typedef std::shared_ptr<masterAtomGroup> ptr;
    masterAtomGroup(std::string,std::string,bool printFileInfo=true);
    ~masterAtomGroup() {};

    // Similar virtuals to Reader class
    virtual void readFrame(int);
    virtual void getPositions(std::vector<float>&,std::vector<float>&,std::vector<float>&);
    virtual void getTypes(std::vector<std::string>&);
    virtual void getMolecules(std::vector<int>&);
    virtual void getBox(std::vector<float>&);
};
#endif
