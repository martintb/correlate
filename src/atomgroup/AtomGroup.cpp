#include <string>
#include <memory>

#include "AtomGroup.hpp"
#include "masterAtomGroup.hpp"

using namespace std;

AtomGroup::ptr AtomGroup::make(string topoFile,string trjFile) {
  return AtomGroup::ptr( new masterAtomGroup(topoFile,trjFile));
}
