#include "geometry.h"

#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#define EPSILON 0.000001
#define IEPSILON 1000000

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

int rayTriangleIntersect (Vec orig, Vec dir, const Vec v0, const Vec v1, const Vec v2, double *t, double *u, double *v) { 
    Vec v0v1 = subvec(v1, v0); 
    Vec v0v2 = subvec(v2, v0); 
    Vec pvec = cross(dir, v0v2);
    float det = dot(v0v1, pvec);
 
    // ray and triangle are parallel if det is close to 0
    if (det < EPSILON)
        return 0; 
 
    float invDet = 1 / det;
 
    Vec tvec = subvec(orig, v0);
    *u = dot(tvec, pvec) * invDet;
    if (*u < 0 || *u > 1)
        return 0; 
 
    Vec qvec = cross(tvec, v0v1);
    *v = dot(dir, qvec) * invDet;
    if (*v < 0 || *u + *v > 1)
        return 0;
 
    *t = dot(v0v2, qvec) * invDet; 

    return *t > 0; 
}
int rayBoxIntersect(Vec o, Vec d, BBox b) {
    // invdir
    Vec i;
    i.x = d.x == 0 ? IEPSILON : 1.0 / d.x;
    i.y = d.y == 0 ? IEPSILON : 1.0 / d.y;
    i.z = d.z == 0 ? IEPSILON : 1.0 / d.z;
    double tmin, tmax, tymin, tymax, tzmin, tzmax;
    if (i.x >= 0) {
        tmin = (b.bmin.x - o.x) * i.x;
        tmax = (b.bmax.x - o.x) * i.x;
    }
    else {
        tmin = (b.bmax.x - o.x) * i.x;
        tmax = (b.bmin.x - o.x) * i.x;
    }

    if (i.y >= 0) {
        tymin = (b.bmin.y - o.y) * i.y;
        tymax = (b.bmax.y - o.y) * i.y;
    }
    else {
        tymin = (b.bmax.y - o.y) * i.y;
        tymax = (b.bmin.y - o.y) * i.y;
    }


    if ((tmin > tymax) || (tymin > tmax))
        return 0;
    if (tymin > tmin)
        tmin = tymin;
    if (tymax < tmax)
        tmax = tymax;

    if (i.z >= 0) {
        tzmin = (b.bmin.z - o.z) * i.z;
        tzmax = (b.bmax.z - o.z) * i.z;
    }
    else {
        tzmin = (b.bmax.z - o.z) * i.z;
        tzmax = (b.bmin.z - o.z) * i.z;
    }

    if ((tmin > tzmax) || (tzmin > tmax))
        return 0;
    if (tzmin > tmin)
        tmin = tzmin;
    if (tzmax < tmax)
        tmax = tzmax;

    return 1;
   
}