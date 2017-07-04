#ifndef _FIGURE_H_
#define _FIGURE_H_

#include "vec3d.h"
#include "globals.h"
#include "line.h"

class Figure {
public:
	Shape shape;
	Material material;

    Figure() : material(), shape(UNDEFINED) {}
	Figure(Shape _shape) : material(), shape(_shape) {}
    Figure(const Material& _material, Shape _shape) : material(_material), shape(_shape) {}
    
    virtual Intersection intersecWithLine(Line& line) = 0;
    virtual Vec3d getNormal(const Vec3d& p) const = 0;
};

class Triangle : public Figure {
public:
    Vec3d p0, p1, p2;
    
    Triangle() : Figure(TRIANGLE),  p0(), p1(), p2() {}
    Triangle(const Material& _material, const Vec3d& _p0, const Vec3d& _p1, const Vec3d& _p2) : Figure(_material, TRIANGLE), p0(_p0), p1(_p1), p2(_p2) {}
    
    Intersection intersecWithLine(Line& line) {
        Vec3d E2 = p2 - p0;
        Vec3d E1 = p1 - p0;
        Vec3d T = line.base - p0;
        Vec3d P = line.dir ^ E2;
        Vec3d Q = T ^ E1;
        double t = (Q * E2) / (P * E1);
        double u = (P * T) / (P * E1);
        double v = (Q * line.dir) / (P * E1);
        if (u < 0 || v < 0 || (u + v > 1) || (t < eps)) {
            return Intersection(false, 0, this);
        }
        else {
            return Intersection(true, t, this);
        }
    }
    
    Vec3d getNormal(const Vec3d& p) const {
        Vec3d ans;
        Vec3d v1 = p1 - p0;
        Vec3d v2 = p2 - p0;
        ans.x = v1.y * v2.z - v1.z * v2.y;
        ans.y = v2.x * v1.z - v1.x * v2.z;
        ans.z = v1.x * v2.y - v2.x * v1.y;
        return ans.normalized();
    }
};

std::istream& operator >> (std::istream& in, Triangle& a) {
    in >> a.p0 >> a.p1 >> a.p2;
    return in;
}

class Sphere : public Figure {
public:
	Vec3d cent;
	double rad;
	
	Sphere() : Figure(SPHERE), cent(), rad(0) {}
	Sphere(const Material& _material, const Vec3d& _cent, const double& _rad) : Figure(_material, SPHERE), cent(_cent), rad(_rad) {}
	
	Intersection intersecWithLine(Line& line) {
        if (line.dist(cent) > rad) 
            return Intersection(false, 0, this);
        Vec3d k = line.base - cent;
        double b = k * line.dir;
        double c = k.norm() - rad * rad;
        double d = b * b - c;
        if (d >= 0) {
            double sqrtd = sqrt(d);

            // t, a == 1
            double t1 = -b + sqrtd;
            double t2 = -b - sqrtd;

            double min_t  = std::min(t1,t2);
            double max_t = std::max(t1,t2);

            double t = (min_t > eps) ? min_t : max_t;

            if (t <= eps)    
                return Intersection(false, 0, this);
            else
                return Intersection(true, t, this);
        }
        return Intersection(false, 0, this);
    }
    
    Vec3d getNormal(const Vec3d& p) const {
        return (p - cent).normalized();
    }
};

#endif // _FIGURE_H_
