#include "pixels.h"
#include "geometry.h"
#include "models.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


#define EPSILON 0.000001
#define IEPSILON 1000000

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
 
    return 1; 
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
Pixel raytrace(Vec raypos, Vec raydir, struct model* pot) {
    Pixel bg = {0, 255, 0};
    // Check acceleration boxes
    if (!rayBoxIntersect(raypos, raydir, pot->bounding))
        return bg;

    // Ok, this ray hits the bounding box. Let's trace polygons.
    double tnear = 10000;
    double t = 10000000;
    double u = 0;
    double v = 0;
    UV uv;
    int hittri = -1;
    Vec p1;
    Vec p2;
    Vec p3;
    for (int i = 0; i < pot->nfaces; i++) {
        p1 = pot->vertices[pot->vertexIndex[i*3]];
        p2 = pot->vertices[pot->vertexIndex[i*3+1]];
        p3 = pot->vertices[pot->vertexIndex[i*3+2]];
        if (rayTriangleIntersect(raypos, raydir, p1, p2, p3, &t, &u, &v) && t < tnear) {
            tnear = t;
            uv = (UV) {u, v};
            hittri = i;
        }
    }
    if (hittri==-1)
        return bg;

    Vec normal;
    if (pot->mode.smooth) {
        p1 = pot->normals[pot->normIndex[hittri*3]];
        p2 = pot->normals[pot->normIndex[hittri*3+1]];
        p3 = pot->normals[pot->normIndex[hittri*3+2]];
        normal = addvec(addvec(scalevec(p1, 1 - uv.u - uv.v), scalevec(p2, uv.u)), scalevec(p3, uv.v));
    }
    else {
        p1 = pot->vertices[pot->vertexIndex[hittri*3]];
        p2 = pot->vertices[pot->vertexIndex[hittri*3+1]];
        p3 = pot->vertices[pot->vertexIndex[hittri*3+2]];
        normal = normalize(cross(subvec(p2, p1), subvec(p3, p1)));
    }

    UV tex;
    if (pot->mode.textures) {
        UV u1 = pot->textures[pot->texIndex[hittri*3]];
        UV u2 = pot->textures[pot->texIndex[hittri*3+1]];
        UV u3 = pot->textures[pot->texIndex[hittri*3+2]];
        tex = wrapUV(combineUV(uv, u1, u2, u3), TILE);
    }

    Pixel V;
    if (pot->mode.textures)
        V = getPixel(pot->texture, (int) (tex.u * pot->texture.W), (int) ((1.0 - tex.v) * pot->texture.H));
    else
        V = (Pixel) {255, 255, 255};

    double d = -dot(normal, raydir);
    if (d < 0)
        d = 0;
    return scalepixel(V, d);

}

void render(Image buffer, struct model* pot) {
    Vec camera = {1, 1, 1};
    Vec cangle = normalize((Vec) {-1, -0.5, -1});
    Vec uu = normalize(cross((Vec) {0, 1, 0}, cangle));
    Vec vv = normalize(cross(cangle, uu));
    double fov_a = tan(1.2/2);
    double aspectRatio = (double) buffer.W / buffer.H;

    double uvx;
    double uvy;
    int tencent = buffer.W / 10;
    for (int x = 0; x < buffer.W; x++) {
        if (x % tencent == 0)
            printf("%d percent \n", 100*x/buffer.W);
        for (int y = 0; y < buffer.H; y++) {
            uvx = (((double)x * 2.0  + 1)/buffer.W - 1) * aspectRatio * fov_a;
            uvy = (1-((double)y * 2.0 + 1)/buffer.H) * fov_a;
            Vec dir = normalize(addvec(addvec(scalevec(uu, uvx),scalevec(vv, uvy)), cangle));
            Pixel V = raytrace(camera, dir, pot);
            if (V.r == 0 && V.g == 255) {
                V = (Pixel) {y * 256 / buffer.H, 256 - (y * 256 / buffer.H), 100};
            }
            change_pixel(buffer, x, y, V);
        }
    }
}

int main(int argc, char* argv[]) {
    // Check arguments to program 
    if (argc < 5 || argc > 8) {
        fprintf(stderr, "Usage: render modelname W H filename [-t texture] [-p]\n");
        return 1;
    }
    unsigned int W = atoi(argv[2]);
    unsigned int H = atoi(argv[3]);
    const char* filename = argv[4];
    const char* name = argv[1];

    // Process mode arguments
    struct renderproperties mode = {1, 0};
    char* texname;
    for (int i = 5; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0) {
            mode.smooth = 0;
        }

        if (strcmp(argv[i], "-t") == 0) {
            if (i < argc-1 && (strncmp(argv[i+1], "-", 1) != 0)) {
                texname = argv[i+1];
                mode.textures = 1;
                if (access(texname, F_OK|R_OK) == -1) {
                    fprintf(stderr, "Texture File doesn't exist\n");
                    return 1;
                }    
            }
        }
    }

    if (W*H == 0) {
        fprintf(stderr, "Bad width or height\n");
        return 1;
    }
    
     // OK, we can run now.
    // Initialize blank canvas
    const Pixel BACKGROUND = {40, 100, 225};
    Image buffer = create_image(W, H, BACKGROUND);
    
    // Load model
    struct model* pot = load_obj(name);
    // Generate a bounding box for optimization
    generate_acceleration(pot);

    mode.smooth = mode.smooth && (pot->f == VNORM || pot->f == VTEXNORM);
    mode.textures = mode.textures && (pot->f == VTEX|| pot->f == VTEXNORM);
    pot->mode = mode;
    if (mode.textures) {
        pot->texture = load_image(texname);
        if (pot->texture.data == NULL)
            return 1;
        printf("Textures enabled.\n");
    }

    if (mode.smooth) {
        printf("Smooth shading enabled.\n");
    }
    render(buffer, pot);
    // Save image to file
    stbi_write_tga(filename, buffer.W, buffer.H, 3, buffer.data);
    // Free model memory
    free_model(pot);
    // Free image
    free_image(buffer);
    return 0;
}
