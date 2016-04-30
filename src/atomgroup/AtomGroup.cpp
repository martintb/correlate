#include <armadillo>
#include <string>
#include <memory>
#include <algorithm>
#include "AtomGroup.hpp"
#include "XML.hpp"
#include "DCD.hpp"
#include "debug.hpp"

using namespace std;

AtomGroup::AtomGroup(string xmlPath, string dcdPath) {
  this->dcdPath = dcdPath;
  this->xmlPath = xmlPath;
  this->init=false; 
  this->frame=-1;
}

AtomGroup::~AtomGroup() {
  if (dptr.unique())
    dptr.reset();
  if (xptr.unique())
    xptr.reset();
};


void AtomGroup::setSize(int natoms) {
  this->natoms = natoms;
  pos.set_size(natoms,3);
  type.reserve(natoms);
}

void AtomGroup::readFiles() {
  dptr = make_shared<DCD>(dcdPath);
  xptr = make_shared<XML>(xmlPath);
  this->init=true;

  if (dptr->natoms != xptr->natoms) {
    cerr << "Error! XML and DCD files don't have the same number of atoms." << endl;
    dptr->printFileInfo();
    xptr->printFileInfo();
    LOC();
    exit(EXIT_FAILURE);
  }
  setSize(dptr->natoms);
  readFrame(0);
}

void AtomGroup::readFrame(int frame) {
  this->frame = frame;
  dptr->readFrame(frame);
  pos = dptr->pos;
  type = xptr->type;
  lx = dptr->lx;
  ly = dptr->ly;
  lz = dptr->lz;
}


AtomGroup::ptr AtomGroup::select(const vector<string>& type_select) {
  vector<string> new_type;
  vector<int> std_uvec;
  for (int i=0; i<natoms; ++i) {
    //search through type_select array for current type
    //if found, append index to sel
    if (find( type_select.begin(), 
              type_select.end(), 
              xptr->type[i]) != type_select.end()){
      std_uvec.push_back(i);
      new_type.push_back(type[i]);
    }
  }
 
  arma::uvec sel = arma::conv_to<arma::uvec>::from(std_uvec);
  AtomGroup::ptr aptr = make_shared<AtomGroup>(xmlPath,dcdPath);
  if (sel.n_elem>0) { 
    mask(aptr,sel); 
    aptr->type = new_type;
  }
  // cout << ".:: Created Sub AtomGroup with size " << aptr->natoms << endl;
  return aptr;
}

int AtomGroup::check_select_size(const vector<string>& type_select) {
  int count=0;
  for (int i=0; i<natoms; ++i) {
    //search through type_select array for current type
    //if found, increment counter
    if (find( type_select.begin(), type_select.end(), xptr->type[i]) != type_select.end()){
      count +=1;
    }
  }
 
  return count;
}

void AtomGroup::mask(AtomGroup::ptr aptr, arma::umat& sel) {
  aptr->setSize(sel.n_elem);
  aptr->frame = frame;
  aptr->lx = lx;
  aptr->ly = ly;
  aptr->lz = lz;
  aptr->pos = pos.rows(sel);
  aptr->init=true;
}

vector<float> AtomGroup::toSTLVec(float dim) {
  return arma::conv_to<vector<float> >::from(pos.col(dim));
}

arma::fvec AtomGroup::toARMAfvec(float dim) {
  return pos.col(dim);
}
 
// void AtomGroup::unwrap_by_image() {
//   for (int frame=0;frame<num_frames;++frame) {
//     arma::mat fimg = arma::conv_to<arma::mat>::from(img.slice(frame)) ;
//     fimg.each_row() %= box.row(frame);
//     pos.slice(frame) += fimg;
//   }
// }
