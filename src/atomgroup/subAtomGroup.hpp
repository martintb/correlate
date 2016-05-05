#ifndef SUB_ATOMGROUP_HPP
#define SUB_ATOMGROUP_HPP
#include <vector>
#include <string>

#include "AtomGroup.hpp"

class subAtomGroup : public AtomGroup {
  public:
    typedef std::shared_ptr<subAtomGroup> ptr;
    subAtomGroup(AtomGroup::ptr,std::vector<int>);
    ~subAtomGroup() {};

    AtomGroup::ptr master;
    std::vector<int> indices;

    // Similar virtuals to Reader class
    virtual AtomGroup::ptr select_types(std::vector<std::string> &selTypes);
    virtual void readFrame(int);
    virtual void getPositions(std::vector<float>&,std::vector<float>&,std::vector<float>&);
    virtual void getTypes(std::vector<std::string>&);
    virtual void getMolecules(std::vector<int>&);
    virtual void getBox(std::vector<float>&);
};
#endif
