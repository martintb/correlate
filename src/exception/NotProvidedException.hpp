#ifndef NOTPROVIDED_HPP
#define NOTPROVIDED_HPP

#include <stdexcept>
#include <string>

class NotProvidedException : public std::logic_error
{
  public:
    NotProvidedException(std::string name, std::string quantity) : 
      std::logic_error((name + " doesn't provide "+ quantity)) {}

};
#endif
