#include "screenshot.h"
#include <GL/glut.h>
#include <vector>
#include <iostream>
#include <cstdio>
#include <cstring>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void saveScreenshotPNG()
{
    static int count = 1;

    int width  = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);

    std::vector<unsigned char> pixels(3 * width * height);
    std::vector<unsigned char> flipped(3 * width * height);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());

    for(int y=0; y<height; y++){
        std::memcpy(&flipped[y*width*3], &pixels[(height-1-y)*width*3], width*3);
    }

    char filename[64];
    std::snprintf(filename, sizeof(filename), "ofet_%03d.png", count++);

    if(stbi_write_png(filename, width, height, 3, flipped.data(), width*3)){
        std::cout << "Saved: " << filename << "\n";
    }else{
        std::cerr << "PNG save failed\n";
    }
}
