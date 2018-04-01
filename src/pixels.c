#include "pixels.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <stdlib.h>
#include <stdio.h>

Image create_image(int W, int H, Pixel v) {
    int size = W*H*3;
    unsigned char* frame = calloc(size, sizeof(char));
    for (int i = 0; i < size; i += 3) {
        frame[i] = v.r;
        frame[i+1] = v.g;
        frame[i+2] = v.b;
    }
    Image buffer = {NULL, W, H};
    buffer.data = frame;
    return buffer;
}

void free_image(Image k) {
    k.W = -1;
    k.H = -1;
    stbi_image_free(k.data);
}

void change_pixel(Image buffer, int X, int Y, Pixel v) {
    int startbyte = byte_at_pixel(buffer.W, buffer.H, X, Y);
    buffer.data[startbyte] = v.r;
    buffer.data[startbyte+1] = v.g;
    buffer.data[startbyte+2] = v.b;
}

Pixel getPixel(Image buffer, int X, int Y) {
    int startbyte = byte_at_pixel(buffer.W, buffer.H, X, Y);

    Pixel v;
    v.r = buffer.data[startbyte];
    v.g = buffer.data[startbyte+1];
    v.b = buffer.data[startbyte+2];

    return v;
    
}

int byte_at_pixel(int W, int H, int X, int Y) {
    if (Y >= H || X >= W)
        return 0;
    return (Y * W * 3) + (X * 3);
}

Pixel scalepixel(Pixel V, float f) {
    return (Pixel) {V.r * f, V.g * f, V.b * f};
}

Image load_image(char* filename) {
    int x,y,n;
    unsigned char* data;
    data = stbi_load(filename, &x, &y, &n, 3);
    if (data == NULL) {
        fprintf(stderr, "Bad file");
    }
    Image buffer = {data, x, y};
    return buffer;
}