#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "pixels.h"
#include "geometry.h"
#include "models.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define EPSILON 0.000001

int rayTriangleIntersect (Vec orig, Vec dir, const Vec v0, const Vec v1, const Vec v2, double *t, double *u, double *v) { 
    Vec v0v1 = subvec(v1, v0); 
    Vec v0v2 = subvec(v2, v0); 
    Vec pvec = cross(dir, v0v2); 
    float det = dot(v0v1, pvec); 
 
    // ray and triangle are parallel if det is close to 0
    if (fabs(det) < EPSILON)
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
Pixel raytrace(Vec raypos, Vec raydir, struct model* pot) {
    // We will trace this ray from the camera until it hits a pyramid.
    int smooth = 0;
    if (pot->f == VNORM || pot->f == VTEXNORM) {
        smooth = 1;
    }

    double tnear = 10000;
    int isect = 0;
    double t = 10000000;
    double uf;
    double vf;
    double u = 0;
    double v = 0;
    int hittri;
    Vec p1;
    Vec p2;
    Vec p3;
    for (int i = 0; i < pot->nfaces; i++) {
        p1 = pot->vertices[pot->vertexIndex[i*3]];
        p2 = pot->vertices[pot->vertexIndex[i*3+1]];
        p3 = pot->vertices[pot->vertexIndex[i*3+2]];
        if (rayTriangleIntersect(raypos, raydir, p1, p2, p3, &t, &u, &v) && t < tnear) {
            tnear = t;
            uf = u;
            vf = v;
            hittri = i;
            isect = 1;
        }
    }
    if (!isect)
        return (Pixel) {0, 255, 0};
    Vec normal;
    if (!smooth) {
        p1 = pot->vertices[pot->vertexIndex[hittri*3]];
        p2 = pot->vertices[pot->vertexIndex[hittri*3+1]];
        p3 = pot->vertices[pot->vertexIndex[hittri*3+2]];
        normal = normalize(cross(subvec(p2, p1), subvec(p3, p1)));
    }
    else {
        p1 = pot->normals[pot->vertexIndex[hittri*3]];
        p2 = pot->normals[pot->vertexIndex[hittri*3+1]];
        p3 = pot->normals[pot->vertexIndex[hittri*3+2]];
        normal = normalize(addvec(addvec(scalevec(p1, 1 - uf - vf), scalevec(p2, uf)), scalevec(p3, vf))); 
    }
    double d = dot(normal, scalevec(normalize(raydir), -1));
    if (d < 0)
        d = 0;
    return (Pixel) {255 * d, 255 * d, 255 * d};

}

void render(Image buffer, struct model* pot) {
    Vec camera = {3, 0, 0};
    Vec cangle = normalize((Vec) {-1, 0, 0});
    Vec uu = normalize(cross((Vec) {0, 1, 0}, cangle));
    Vec vv = normalize(cross(cangle, uu));
    double fov_a = tan(1.35/2);
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
    if (argc != 5) {
        fprintf(stderr, "Usage: render modelname W H filename\n");
        return 1;
    }
    unsigned int W = atoi(argv[2]);
    unsigned int H = atoi(argv[3]);
    const char* filename = argv[4];
    const char* name = argv[1];

    if (W*H == 0) {
        fprintf(stderr, "Bad width or height\n");
        return 1;
    } // OK, we can run now.
    // Initialize blank canvas
    const Pixel BACKGROUND = {40, 100, 225};
    Image buffer = create_image(W, H, BACKGROUND);

    // Load model
    struct model* pot = load_obj(name);
    if (pot->f == VNORM || pot->f == VTEXNORM) {
        printf("Found normals. Enabling smooth shading...\n");
    }
    render(buffer, pot);
    // Save image to file
    stbi_write_bmp(filename, buffer.W, buffer.H, 3, buffer.data);
    // Free model memory
    free_model(pot);
    // Free image
    free_image(buffer);
    return 0;
}
