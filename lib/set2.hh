#ifndef __set2_hh__
#define __set2_hh__

#include <set1.hh>
#include <stream2string.h>

template <class Container> 
inline Set<Container>::operator std::string () {
  return stream2string<std::string>(*this);
}

#endif
