#ifndef INCLUDE_MODELS
#define INCLUDE_MODELS

#include "geometry.h"
#include "pixels.h"

enum fformat {NONE, VERT, VTEX, VNORM, VTEXNORM};

struct renderproperties {
    int smooth;
    int textures;
    int normalmap;
};

struct model {
    int nverts;
    int nfaces;
    int ntex;

    Vec* vertices;
    UV* textures;
    Vec* normals;

    Vec* tangent;
    Vec* bitangent;

    int* vertexIndex;
    int* texIndex;
    int* normIndex;

    enum fformat f;
    struct renderproperties mode;

    Image texture;
    Image normalmap;
};


struct model* load_obj(const char* name);
void free_model(struct model* model);
#endif