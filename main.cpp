#include <algorithm>
#include <cmath>
#include <png++/png.hpp>

#include "vec3d.h"
#include "light.h"
#include "line.h"
#include "globals.h"
#include "figure.h"

using namespace std;

Intersection nearestIntersection(Line& line, const vector<Figure*>& list, const Figure* a) {
    Intersection best = Intersection(false, 0, NULL);
	for (int t = 0; t < (int)list.size(); t++) {
        if (list[t] == a)
            continue;
		Intersection cur = list[t]->intersecWithLine(line);
		if (!best.is || (cur.is && cur.dist < best.dist)) {
			best = cur;
		}
	}
	return best;
}

double countKoeffFromLights(Figure* figure, const Vec3d& point, const vector<Figure*>& a, const vector<Light>& lights) {
	double koeff = 0;
	for (int t = 0; t < (int)lights.size(); t++) {
		Line lineFromLight = Line(lights[t].cent, point);
		Intersection best2 = nearestIntersection(lineFromLight, a, NULL);
		double dist = point.distToPoint(lights[t].cent);
		if (!(best2.is && best2.dist + eps < dist)) {
			double koeff_cos = 1;
			Vec3d v1 = figure->getNormal(point);
			if (!(v1.x == 1 && v1.y == 1 && v1.z == 1)) {
				Vec3d v2 = (lights[t].cent - point).normalized();
				koeff_cos = fabs(v1.getCos(v2));
			}
			koeff += (lights[t].brightness * 40) / (dist * dist) * koeff_cos;
		}
	}
	//koeff = min(double(1), koeff);
	koeff = 0.5 + koeff * 0.5;
	return koeff;
}

pair<bool, Vec3d> getReflect(const Vec3d& dir, Vec3d normal, double a) {
	double cos = -(normal * dir);
	if (cos < -eps) {	
		cos *= -1;
		normal *= -1;
		a = 1.0 / a;
	}
	double k = 1.0 - a * a * (1.0 - cos	* cos);
	if (k > eps)
		return make_pair(true, (dir * a + normal * (a * cos - sqrt(k))).normalized());
	else
		return make_pair(false, Vec3d(1, 1, 1));
}

Vec3d getColor(Line& line, const vector<Figure*>& a, const vector<Light>& lights, int depth) {
	Intersection best = nearestIntersection(line, a, NULL);
	//no intersection
	if (!best.is) {
		return Vec3d(255, 255, 255);
	}
	Vec3d curPoint = line.base + line.dir * best.dist;
	double koeff = countKoeffFromLights(best.figure, curPoint, a, lights);
	Vec3d color = best.figure->material.color * koeff;

	if (color.x > 255)
		color.x = 255;
	if (color.y > 255)
		color.y = 255;
	if (color.z > 255)
		color.z = 255;

	if (depth > 0 && best.figure->material.prelom > 0) {
		Vec3d norm = best.figure->getNormal(curPoint);
		pair<bool, Vec3d> reflection = getReflect(line.dir, norm, (1.0 / best.figure->material.prelom));
		if (reflection.first) {
			Line reflectedLine(curPoint, curPoint + reflection.second);
			color = getColor(reflectedLine, a, lights, depth - 1);		
		}		
	}
	if (depth > 0 && best.figure->material.refractionAbility > 0) {
		Vec3d norm = best.figure->getNormal(curPoint);
		Vec3d refr = line.dir.getRefraction(norm);
		Line lineRec = Line(curPoint, curPoint + refr);
		Vec3d colorRec = getColor(lineRec, a, lights, depth - 1);
		color = color * (1 - best.figure->material.refractionAbility) + colorRec * best.figure->material.refractionAbility;
	}

	return color;
}

/*void test() {
	Triangle a(Material(Vec3d(255, 255, 255), 0, 0), Vec3d(0, 0, 10), Vec3d(10, 10, 10), Vec3d(10, 0, 10));
	Vec3d p0(1, 0, 1);
	Vec3d p1(8, 8, 0);
	Vec3d v1 = p1 - p0;
	Vec3d ans = v1.getRefraction(a.getNormal(v1));

	printf("%.3f %.3f %3.f\n", ans.x, ans.y, ans.z);
}*/

int main() {
	int t1;
	cin >> t1;
	vector<Figure*> a;
	for (int i = 0; i < t1; i++) {
        Vec3d p0, p1, p2;
		Material material;
        cin >> material >> p0 >> p1 >> p2;
        a.push_back(new Triangle(material, p0, p1, p2));
    }
    int t2;
	cin >> t2;
	for (int i = 0; i < t2; i++) {
        Vec3d p0;
		Material material;
        double rad;
        cin >> material >> p0 >> rad;
        a.push_back(new Sphere(material, p0, rad));
    }
    int t3;

    cin >> t3;
    vector<Light> lights(t3);
    for (int i = 0; i < t3; i++) {
        cin >> lights[i];
    }
	
	int n, m;
	cin >> n >> m;
	Vec3d p1, p2, p0;
	cin >> p1 >> p2 >> p0;

	//test();
    
    png::image< png::rgb_pixel > image(m, n);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++) {
			double di = (double(i + 1) / n - double(i) / n) / 2;
			double dj = (double(j + 1) / m - double(j) / m) / 2;
			int k = 0;
			Vec3d color_all(0, 0, 0);
			for (double i2 = double(i) / n; i2 < double(i + 1) / n; i2 += di)
				for (double j2 = double(j) / m; j2 < double(j + 1) / m; j2 += dj) {
					Vec3d cur = p1 + Vec3d(p2.x - p1.x, 0, 0) * i2 + Vec3d(0, p2.y - p1.y, 0) * j2;
					Line line = Line(p0, cur);
					Vec3d color = getColor(line, a, lights, 6);
					color_all += color;
					k++;
				}

			Vec3d color = color_all / k;
/*			Vec3d cur = p1 + Vec3d(p2.x - p1.x, 0, 0) * ((double)i / n) + Vec3d(0, p2.y - p1.y, 0) * ((double)j / m);
			Line line = Line(p0, cur);
            Vec3d color = getColor(line, a, lights, 6);*/
            image[m - j - 1][i] = png::rgb_pixel(color.x, color.y, color.z);
		}

	image.write("picture.png");
}
