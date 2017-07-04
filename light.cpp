#ifndef _LIGHT_CPP_
#define _LIGHT_CPP_

#include "light.h"

std::istream& operator >> (std::istream& in, Light& a) {
    in >> a.cent >> a.brightness;
    return in;
}

#endif //_LIGHT_CPP_