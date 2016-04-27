#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <boost/spirit/include/qi.hpp>
#include <boost/algorithm/string.hpp>
#include "rapidxml.hpp"
#include <functional>

#include "Reader.hpp"
#include "XML.hpp"

using namespace std;

#define TYPES Reader::POSITION | Reader::BOX | Reader::TYPE  | Reader::IMAGE | Reader::VELOCITY
XML::XML(string fileName) : Reader("XML",TYPES) {
  xmlCharArray = nullptr;
  setFilename(fileName);
  read();
  parse();
}


void XML::setFilename(string fileName) { 
  exists(fileName);
  pos.reset();
  vel.reset();
  img.reset();
  type.clear(); // type is a vector<string> all others are armadillo types

  this->fileName = fileName;
  has_read=false;
};


void XML::read() {
  ifstream in(fileName.c_str()); // open file
  in.seekg(0,ifstream::end); // move file ptr to end of file
  long size = in.tellg(); // get size of file by file ptr location
  in.seekg(0); // reset file ptr to beginning
  xmlCharArray.reset(new char[size+1], /*deleter=*/std::default_delete<char[]>() );
  in.read(xmlCharArray.get(),size);
  xmlCharArray.get()[size] = '\0'; // don't forget the terminating 0
  has_read=true;
  in.close();
};

void XML::parse() {
  if(has_read == false) this->read();
  xmlFile.parse<0>(xmlCharArray.get());
  rapidxml::xml_node<> *head_node;
  rapidxml::xml_node<> *node;
  rapidxml::xml_attribute<> *attr;
  char* temp;
  int num;

  head_node = xmlFile.first_node("hoomd_xml");
  assert(head_node);

  head_node = head_node->first_node("configuration");
  assert(head_node);
  istringstream(head_node->first_attribute("natoms")->value()) >> natoms;
  istringstream(head_node->first_attribute("time_step")->value()) >> frame;

  node = head_node->first_node("box");
  assert(node);
  istringstream(node->first_attribute("lx")->value()) >> lx;
  istringstream(node->first_attribute("ly")->value()) >> ly;
  istringstream(node->first_attribute("lz")->value()) >> lz;

  node = head_node->first_node("position");
  assert(node);
  string pos_str(node->value());
  pos.set_size(natoms,3);
  parse_node<float>(pos_str,pos,"float");

  // node = head_node->first_node("velocity");
  // assert(node);
  // string vel_str(node->value());
  // vel.set_size(natoms,3);
  // parse_node<float>(vel_str,vel,"float");

  node = head_node->first_node("image");
  assert(node);
  string img_str(node->value());
  img.set_size(natoms,3);
  parse_node<int>(img_str,img,"int");

  node = head_node->first_node("type");
  assert(node);
  //istringstream type_stream(node->value());
  string type_str(node->value());
  type.reserve(natoms);
  boost::split(type,type_str,boost::is_any_of("\n "));
  // type.erase(remove_if(type.begin(),type.end(),XML::IsEmptyStr()),type.end());
  type.erase(remove_if(type.begin(),type.end(),mem_fun_ref(&string::empty)),type.end());
}



float XML::parse_float(char const* first, char const* last) {
  float n;
  namespace qi = boost::spirit::qi;
  using qi::float_;
  qi::parse(first, last, float_, n);
  return n;
}

double XML::parse_double(char const* first, char const* last) {
  double n;
  namespace qi = boost::spirit::qi;
  using qi::double_;
  qi::parse(first, last, double_, n);
  return n;
}

int XML::parse_int(char const* first, char const* last) {
  int n;
  namespace qi = boost::spirit::qi;
  using qi::int_;
  qi::parse(first, last, int_, n);
  return n;
}


void XML::printFileInfo() {
  cout << "File Name:        " << fileName  << endl;
  cout << "Number of Atoms:  " << natoms  << endl;
  cout << "Frame Number:     " << frame  << endl;
}

