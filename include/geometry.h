#ifndef INCLUDE_GEOM
#define INCLUDE_GEOM

typedef struct vec3 {
    double x;
    double y;
    double z;
} Vec; // point or vector

typedef struct box {
    Vec bmin;
    Vec bmax;
} BBox;

typedef struct vec2 {
    double u;
    double v;
 } UV;

enum uvmode {TILE, CLAMP, MIRRORTILE};

Vec addvec(Vec p1, Vec p2);
Vec subvec(Vec p1, Vec p2);
Vec scalevec(Vec p, double s);

double dot(Vec p1, Vec p2);
Vec cross(Vec p1, Vec p2);

Vec normalize(Vec p);
double vecmag(Vec p);

UV addUV(UV p1, UV p2);
UV scaleUV(UV p, double d);
UV wrapUV(UV p, enum uvmode u);
UV combineUV(UV interp, UV u1, UV u2, UV u3);

int rayTriangleIntersect (Vec orig, Vec dir, const Vec v0, const Vec v1, const Vec v2, double *t, double *u, double *v);
int rayBoxIntersect(Vec o, Vec d, BBox b);
#endif