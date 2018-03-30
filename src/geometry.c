#include "geometry.h"
#include <stdlib.h>
#include <math.h>

Vec addvec(Vec p1, Vec p2) { return (Vec) {p1.x+p2.x, p1.y+p2.y, p1.z+p2.z}; }
Vec subvec(Vec p1, Vec p2) { return (Vec) {p1.x-p2.x, p1.y-p2.y, p1.z-p2.z}; }
Vec scalevec(Vec p, double s) { return (Vec) {p.x*s, p.y*s, p.z*s}; }
double dot(Vec p1, Vec p2) { return p1.x*p2.x + p1.y*p2.y + p1.z*p2.z; }
Vec cross(Vec p1, Vec p2) {
    Vec p3;
    p3.x = p1.y*p2.z - p1.z*p2.y;
    p3.y = p1.z*p2.x - p1.x*p2.z;
    p3.z = p1.x*p2.y - p1.y*p2.x;
    return p3;
}
Vec normalize(Vec p) { return scalevec(p, 1/vecmag(p)); }
double vecmag(Vec p) { return sqrt(p.x*p.x + p.y*p.y + p.z*p.z); }


