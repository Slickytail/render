#ifndef INCLUDE_MODELS
#define INCLUDE_MODELS

#include "geometry.h"
#include "pixels.h"

enum fformat {NONE, VERT, VTEX, VNORM, VTEXNORM};

struct renderproperties {
    int smooth;
    int textures;
};

typedef struct box {
    Vec bmin;
    Vec bmax;
} BBox;

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

    struct box bounding;

    enum fformat f;
    struct renderproperties mode;

    Image texture;
};


struct model* load_obj(const char* name);
void generate_acceleration(struct model* model);
void free_model(struct model* model);
#endif