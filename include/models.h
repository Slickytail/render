#ifndef INCLUDE_MODELS
#define INCLUDE_MODELS

#include "geometry.h"

struct model {
    int nverts;
    Polygon vertices[];
};

const struct model pyramid = {6, {
    (Polygon) { {-1, 0, -1}, {-1, 0, 1}, {1, 0, -1} },
    (Polygon) { {1,  0, 1},  {-1, 0, 1}, {1, 0, -1} },
    (Polygon) { {1,  0, 1},  {0,  1, 0}, {1, 0, -1} },
    (Polygon) { {1,  0, 1},  {0,  1, 0}, {-1, 0, 1} },
    (Polygon) { {-1, 0, -1}, {0,  1, 0}, {-1, 0, 1} },
    (Polygon) { {-1, 0, -1}, {0,  1, 0}, {1, 0, -1} } },
};


#endif