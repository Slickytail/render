#ifndef INCLUDE_MODELS
#define INCLUDE_MODELS

#include "geometry.h"

struct model {
    int nverts;
    int nfaces;
    Vec* vertices;
    Vec* normals;
    int* vertexIndex;
};
struct model pyramid;

struct model* load_obj(const char* name);
#endif