#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "pixels.h"
#include "geometry.h"
#include "models.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define EPSILON 0.000001

int rayTriangleIntersect (Vec orig, Vec dir, const Polygon* tri, double *t, double *u, double *v) { 
    Vec v0v1 = subvec(tri->b, tri->a); 
    Vec v0v2 = subvec(tri->c, tri->a); 
    Vec pvec = cross(dir, v0v2); 
    float det = dot(v0v1, pvec); 
 
    // ray and triangle are parallel if det is close to 0
    if (fabs(det) < EPSILON)
        return 0; 
 
    float invDet = 1 / det; 
 
    Vec tvec = subvec(orig, tri->a); 
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
Pixel raytrace(Vec raypos, Vec raydir) {
    // We will trace this ray from the camera until it hits a pyramid.
    double tnear = 10000;
    int isect = 0;
    double t = 10000000;
    double uf;
    double vf;
    double u = 0;
    double v = 0;
    int k;
    for (int i = 0; i < pyramid.nverts; i++) {
        if (rayTriangleIntersect(raypos, raydir, &(pyramid.vertices[i]), &t, &u, &v) && t < tnear) {
            tnear = t;
            uf = u;
            vf = v;
            k = i;
            isect = 1;
        }
    }
    if (!isect)
        return (Pixel) {40, 100, 225};
    Polygon it = pyramid.vertices[k];
    Vec normal = normalize(cross(subvec(it.b, it.a), subvec(it.c, it.a)));
    double d = dot(normal, scalevec(normalize(raydir), -1));
    if (d < 0)
        d = 0;
    return (Pixel) {255 * d, 255 * d, 255 * d};

}

void render(Image buffer) {
    Vec camera = {2, 2, 0.5};
    Vec cangle = normalize((Vec){-2, -1, -0.2});
    Vec uu = normalize(cross((Vec) {0, 1, 0}, cangle));
    Vec vv = normalize(cross(cangle, uu));
    double fov_a = tan(1.13446/2);
    double aspectRatio = (double) buffer.W / buffer.H;

    double uvx;
    double uvy;

    for (int x = 0; x < buffer.W; x++) {
        for (int y = 0; y < buffer.H; y++) {
            uvx = (((double)x * 2.0  + 1)/buffer.W - 1) * aspectRatio * fov_a;
            uvy = (1-((double)y * 2.0 + 1)/buffer.H) * fov_a;
            Vec dir = normalize(addvec(addvec(scalevec(uu, uvx),scalevec(vv, uvy)), cangle));
            Pixel V = raytrace(camera, dir);
            change_pixel(buffer, x, y, V);
        }
    }
}

int main(int argc, char* argv[]) {
    // Check arguments to program 
    if (argc != 4) {
        fprintf(stderr, "Usage: render W H filename\n");
        return 1;
    }
    unsigned int W = atoi(argv[1]);
    unsigned int H = atoi(argv[2]);
    const char* filename = argv[3];

    if (W*H == 0) {
        fprintf(stderr, "Bad width or height\n");
        return 1;
    } // OK, we can run now.

    // Initialize blank canvas
    const Pixel BACKGROUND = {40, 100, 225};
    Image buffer = create_image(W, H, BACKGROUND);
    render(buffer);
    // Save image to file
    stbi_write_bmp(filename, buffer.W, buffer.H, 3, buffer.data);
    
}
