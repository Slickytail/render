#ifndef INCLUDE_MODELS
#define INCLUDE_MODELS

#include "geometry.h"

enum fformat {NONE, VERT, VTEX, VNORM, VTEXNORM};

struct model {
    int nverts;
    int nfaces;
    int ntex;
    Vec* vertices;
    Vec* textures;
    Vec* normals;
    int* vertexIndex;
    int* texIndex;
    int* normIndex;
    enum fformat f;
};

struct model* load_obj(const char* name);
void free_model(struct model* model);
#endif