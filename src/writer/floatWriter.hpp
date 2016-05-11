#ifndef FLOAT_WRITER_HPP
#define FLOAT_WRITER_HPP

#include "Writer.hpp"
#include "Config.hpp"

class floatWriter : public Writer
{
  public:
    floatWriter(Config::ptr conf) : Writer(conf) {
      reset();
    };
    ~floatWriter() {};

    virtual void reset();
    virtual void gather();
};

#endif
