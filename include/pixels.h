#ifndef INCLUDE_PIXEL
#define INCLUDE_PIXEL
typedef struct rgb_pixel {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} Pixel;

typedef struct pixelbuffer {
    unsigned char* data;
    unsigned int W;
    unsigned int H;
} Image;

Image create_image(unsigned int W, unsigned int H, Pixel v);
void change_pixel(Image buffer, unsigned int X, unsigned int Y, Pixel V);
Pixel getPixel(Image buffer, unsigned int X, unsigned int Y);

#endif