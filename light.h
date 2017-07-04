#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "vec3d.h"

class Light {
public:
    Vec3d cent;
    double brightness;
    
    Light() : cent(), brightness(0) {}
    Light(const Vec3d& _cent, int _brightness) : cent(_cent), brightness(_brightness) {}
};

std::istream& operator >> (std::istream& in, Light& a);
 
#endif // _LIGHT_H_