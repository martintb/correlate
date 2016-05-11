#ifndef WRITER_HPP
#define WRITER_HPP

#include <string>
#include <memory>
#include <vector>

#include "Config.hpp"

class Writer {
  public: 
    typedef std::shared_ptr<Writer> ptr;
    Writer(Config::ptr);
    ~Writer() {};
    static ptr get(Config::ptr);


    Config::ptr conf;
    std::vector<unsigned long> vecInt;
    std::vector<float> vecFloat;
    std::vector<float> vecMaster; // final storage vector before output

    std::vector<float> box;
    unsigned long step_count;
    unsigned long pair_count;
    unsigned long pair_count_master; // for mpi transfer


    std::vector<float> coeffAdd; //coefficients to add to vecMaster before output
    std::vector<float> coeffMult;//coefficients to scale vecMaster by before output
    void buildCoeff();

    std::string nextFileName(std::vector<std::string>&);
    void write();
    void writeHorizontal();
    void writeVertical();

    virtual void reset()=0;
    virtual void gather()=0;



};

#endif
