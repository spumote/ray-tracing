#ifndef _GLOBALS_H_
#define _GLOBALS_H_

const double eps = 1e-3;
enum Shape {UNDEFINED = -1, SPHERE, TRIANGLE};

#include "vec3d.h" 

class Figure;

class Intersection {
public:
    bool is;
    double dist;
    Figure* figure;
    
    Intersection() : is(false), dist(0), figure(NULL) {}
    Intersection(const bool _is, const double _dist, Figure* _figure) : is(_is), dist(_dist), figure(_figure) {}
};

class Material {
public:
	Vec3d color;
	double refractionAbility;
	double prelom;

	Material() : color(0, 255, 255), refractionAbility(0.0), prelom(0) {}
	Material(const Vec3d& _color, double _r, double _p) : color(_color), refractionAbility(_r), prelom(_p) {}
};

std::istream& operator >> (std::istream& in, Material& a) {
    in >> a.color >> a.refractionAbility >> a.prelom;
    return in;
}

#endif // _GLOBALS_H_
