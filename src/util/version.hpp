#ifndef VERISON_HPP
#define VERISON_HPP

// The lmpReaders make heavy use of the regex functionality 
// which was only *properly* implemented in gcc 4.9+. The fun part
// is that earlier versions of gcc will allow you to import "broken"
// versions of the regex library. Fun stuff. 
#ifdef __GNUG__
  #define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100  + __GNUC_PATCHLEVEL__)
  #if GCC_VERSION < 40900 //test for GCC>4.9 
    #error "GCC VERSION must be >4.9 to use lmpData Readers!"
  #endif
#endif

#define xstr(s) str(s)
#define str(s) #s

extern const char* version;
extern const char* build_date;

#endif
