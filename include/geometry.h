#ifndef INCLUDE_GEOM
#define INCLUDE_GEOM
typedef struct vec3 {
    double x;
    double y;
    double z;
} Vec; // point or vector
typedef struct triangle {
    struct vec3 a;
    struct vec3 b;
    struct vec3 c;
    // Normal is calculated as as cross product between (a->b) and (a->c)
} Polygon;

Vec addvec(Vec p1, Vec p2);
Vec subvec(Vec p1, Vec p2);
Vec scalevec(Vec p, double s);

double dot(Vec p1, Vec p2);
Vec cross(Vec p1, Vec p2);

Vec normalize(Vec p);
double vecmag(Vec p);

#endif