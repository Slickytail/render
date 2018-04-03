#ifndef INCLUDE_LIGHTS
#define INCLUDE_LIGHTS

#include "pixels.h"
#include "geometry.h"

typedef struct distantlight {
    Vec dir;
    Vec color;
    double intensity;
} DirLight;


#endif