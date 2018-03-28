#include "pixels.h"
#include <stdlib.h>

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