#ifndef _LINE_H_
#define _LINE_H_

#include "vec3d.h"

class Line {
public:
    Vec3d base, dir;

	Line() : base(), dir() {}
	Line(const Vec3d& p1, const Vec3d& p2) : base(p1), dir((p2 - p1).normalized()) {}
	
	double dist(const Vec3d& point) {
        return ((point - base) ^ dir).len() / dir.len();
    }
};

#endif // _LINE_H_