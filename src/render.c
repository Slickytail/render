#include "pixels.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <stdio.h>
#include <stdlib.h>

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
    const Pixel BACKGROUND = {127, 0, 0};
    Image buffer = create_image(W, H, BACKGROUND);

    // Save image to file
    stbi_write_bmp(filename, buffer.W, buffer.H, 3, buffer.data);
    
}
