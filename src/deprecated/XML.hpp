#ifndef XML_HPP
#define XML_HPP

#include <iostream>
#include <armadillo>
#include <string>
#include <vector>
#include <memory>

#include "rapidxml.hpp"
#include "Reader.hpp"
#include "debug.hpp"

class XML : public Reader {
  public:
  // protected:
    std::string fileName;

    double lx;
    double ly;
    double lz;
    arma::Mat<int> mol;
    arma::Mat<float> pos;
    arma::Mat<float> vel;
    arma::Mat<int> img;
    std::vector<std::string> type;
    int natoms;
    int frame;


  // public:
    typedef std::shared_ptr<XML> ptr;

    // derived virtuals
    void readFrame(int frame) {
        std::cout << "XML File is read in constructor! Nothing to read!" << std::endl;
    };
    void printFileInfo();
    arma::Mat<float> getPositions() {return pos;};
    double getLx() {return lx;};
    double getLy() {return ly;};
    double getLz() {return lz;};
    // std::vector<std::string> getTypes() {return type;};

    XML(std::string) ;
    void read();
    void parse();
    void setFilename(std::string);
    int parse_int(char const*,char const*);
    double parse_double(char const*,char const*);
    float parse_float(char const*,char const*);

    std::shared_ptr<char> xmlCharArray;
    bool has_read;
    rapidxml::xml_document<> xmlFile;

    struct IsEmptyStr {
      bool operator()(const std::string& s) {
        return s.find("", 0) != std::string::npos;
      }
    };


    template<typename T>
    void parse_node(std::string& data_str, arma::Mat<T>& array, std::string type) {
      char const* delims = "\n ";
      int offset =0;
      if (data_str[0]=='\n') { offset = 1; }
      char const* val_start = data_str.c_str()+offset;
      char const* val_end = strpbrk(val_start+1, delims);
      int row=0; int col=0;
      for( ;val_end!= NULL; val_end=strpbrk(val_start,delims)) {
        if (row>=array.n_rows) {
          std::cerr << "Attempting to insert data past array size!" << std::endl; 
          std::cerr << "Array  size: " << array.n_rows << std::endl; 
          LOC();
          exit(EXIT_FAILURE);
        }

        if (type=="double") {
          array(row,col) = parse_double(val_start,val_end);
        } else if (type=="float") {
          array(row,col) = parse_float(val_start,val_end);
        } else if (type=="int") {
          array(row,col) = parse_int(val_start,val_end);
        } else {
          std::cerr << "Type specification for conversion not recognized!" << std::endl; 
          std::cerr << "Type  spec: " << type << std::endl; 
          LOC();
          exit(EXIT_FAILURE);
        }
    
        // std::cout << "(row,col): (" << row << "," << col << ") ";
        // std::cout << "val: " << array(row,col) << std::endl;
        val_start = val_end+1;
        col+=1;
        if (col>=array.n_cols) { col=0; row+=1; }
      }
    }
};
#endif
