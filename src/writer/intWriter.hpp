#ifndef INT_WRITER_HPP
#define INT_WRITER_HPP

#include "Writer.hpp"
#include "Config.hpp"

class intWriter : public Writer
{
  public:
    intWriter(Config::ptr conf) : Writer(conf) {
      reset();
    };
    ~intWriter() {};

    virtual void reset();
    virtual void gather();
};

#endif
