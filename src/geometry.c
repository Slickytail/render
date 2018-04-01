#include "geometry.h"
#include "models.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

Vec addvec(Vec p1, Vec p2) { return (Vec) {p1.x+p2.x, p1.y+p2.y, p1.z+p2.z}; }
Vec subvec(Vec p1, Vec p2) { return (Vec) {p1.x-p2.x, p1.y-p2.y, p1.z-p2.z}; }
Vec scalevec(Vec p, double s) { return (Vec) {p.x*s, p.y*s, p.z*s}; }
double dot(Vec p1, Vec p2) { return p1.x*p2.x + p1.y*p2.y + p1.z*p2.z; }
Vec cross(Vec p1, Vec p2) { return (Vec) {p1.y*p2.z - p1.z*p2.y, p1.z*p2.x - p1.x*p2.z, p1.x*p2.y - p1.y*p2.x}; }
Vec normalize(Vec p) { return scalevec(p, 1/vecmag(p)); }
double vecmag(Vec p) { return sqrt(p.x*p.x + p.y*p.y + p.z*p.z); }

UV addUV(UV p1, UV p2) { return (UV) {p1.u+p2.u, p1.v+p2.v}; }
UV scaleUV(UV p, double d) { return (UV) {p.u * d, p.v * d}; }
UV wrapUV(UV p, enum uvmode u) {
    if ((p.u >= 0.0) && (p.u < 1.0) && (p.v >= 0.0) && (p.v < 1.0))
        return p;
    
    if (u == CLAMP) {
        return (UV) {fmax(0, fmin(1, p.u)), fmax(0, fmin(1, p.v))};
    }
    if (u == TILE) {
        return (UV) {p.u - floor(p.u), p.v - floor(p.v)};
    }
    if (u == MIRRORTILE) {
        return (UV) {fabs(p.u), fabs(p.v)};
    }
    return p;
}
UV combineUV(UV interp, UV u1, UV u2, UV u3) {
    u1 = scaleUV(u1, 1.0-interp.u-interp.v);
    u2 = scaleUV(u2, interp.u);
    u3 = scaleUV(u3, interp.v);
    return addUV(addUV(u1, u2), u3);
}