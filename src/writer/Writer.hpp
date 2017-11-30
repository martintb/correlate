#ifndef WRITER_HPP
#define WRITER_HPP

#include <string>
#include <memory>
#include <vector>

#include "Config.hpp"
#include "types.hpp"

class Writer {
  public: 
    typedef std::shared_ptr<Writer> ptr;
    Writer(Config::ptr);
    ~Writer() {};
    static ptr get(Config::ptr&);


    Config::ptr conf;
    std::vector<int_type> vecInt;
    std::vector<float_type> vecFloat;
    std::vector<float_type> vecMaster; // final storage vector before output

    std::vector<float_type> box;
    int_type step_count;
    int_type pair_count;
    int_type pair_count_master; // for mpi transfer


    std::vector<float_type> coeffAdd; //coefficients to add to vecMaster before output
    std::vector<float_type> coeffMult;//coefficients to scale vecMaster by before output
    void buildCoeff();

    std::string nextFileName(std::vector<std::string>&);
    void write(bool vertical=true,bool reset=true);
    void writeHorizontal(bool);
    void writeVertical(bool);
    void writeHeader(float_type);
    bool file_opened;

    virtual void reset()=0;
    virtual void gather()=0;



};

#endif
