#ifndef MASTER_ATOMGROUP_HPP
#define MASTER_ATOMGROUP_HPP
#include <vector>
#include <string>
#include <memory>

#include "AtomGroup.hpp"

class masterAtomGroup : public AtomGroup ,
                        public std::enable_shared_from_this<masterAtomGroup>
{
  public:
    typedef std::shared_ptr<masterAtomGroup> ptr;
    masterAtomGroup(std::string,std::string,bool printFileInfo=true);
    ~masterAtomGroup() {};
    virtual AtomGroup::ptr select_types(std::string &selStr);
    virtual AtomGroup::ptr select_types(std::vector<std::string> &selTypes);

    bool init;
    std::string topoFile;
    std::string trjFile;

    // Similar virtuals to Reader class
    virtual void readFrame(int);
    virtual void getPositions(std::vector<float>&,std::vector<float>&,std::vector<float>&);
    virtual void getTypes(std::vector<std::string>&);
    virtual void getMolecules(std::vector<int>&);
    virtual void getBox(std::vector<float>&);
};
#endif
