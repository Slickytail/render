#ifndef INCLUDE_MODELS
#define INCLUDE_MODELS

#include "geometry.h"
#include "pixels.h"

enum fformat {NONE, VERT, VTEX, VNORM, VTEXNORM};

struct model {
    int nverts;
    int nfaces;
    int ntex;

    Vec* vertices;
    UV* textures;
    Vec* normals;
    int* vertexIndex;
    int* texIndex;
    int* normIndex;

    enum fformat f;

    int hasTexture;
    Image texture;
};

struct renderproperties {
    int smooth;
    int textures;
};

struct model* load_obj(const char* name);
void free_model(struct model* model);
#endif