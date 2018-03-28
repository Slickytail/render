#include "pixels.h"
#include <stdlib.h>

Image create_image(unsigned int W, unsigned int H, Pixel v) {
    int size = W*H*3;
    unsigned char* buffer = calloc(size, sizeof(char));
    for (int i = 0; i < size; i += 3) {
        buffer[i] = v.r;
        buffer[i+1] = v.g;
        buffer[i+2] = v.b;
    }
    Image frame = {NULL, W, H};
    frame.data = buffer;
    return frame;
}

void change_pixel(Image buffer, unsigned int X, unsigned int Y, Pixel V) {
    if (Y >= buffer.H || X >= buffer.W)
        return;
    int startbyte = (Y * buffer.W * 3) + (X * 3);
    buffer[startbyte] = V.r;
    buffer[startbyte+1] = V.g;
    buffer[startbyte+2] = V.b;
}