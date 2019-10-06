#include <iostream>
#include <cassert>

#define CTHROW(m) \
{ std::cerr << m << std::endl; assert(false); }
