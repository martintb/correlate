#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <iostream>

#define LOC() ( std::cerr << "Code was terminated at " << __FILE__ << ":" << __LINE__ << std::endl)

#ifdef DEBUG_MODE
#define DOUT(x) (std::cerr << __FILE__ << ":" << __LINE__ << " " << x << std::endl)
#define DEBUG(x) x
#else
#define DOUT(x)
#define DEBUG(x)
#endif

#endif
