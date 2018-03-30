#ifndef INCLUDE_PIXEL
#define INCLUDE_PIXEL
typedef struct rgb_pixel {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} Pixel;

typedef struct pixelbuffer {
    unsigned char* data;
    int W;
    int H;
} Image;

Image create_image(int W, int H, Pixel v);
void change_pixel(Image buffer, int X, int Y, Pixel v);
Pixel getPixel(Image buffer, int X, int Y);

int byte_at_pixel(int W, int H, int X, int Y);

#endif