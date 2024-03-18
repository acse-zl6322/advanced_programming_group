#ifndef IMAGE2D_H
#define IMAGE2D_H
#include <iostream>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

class Image2D{
public:
    Image2D(const char* from):from(from){
        if(data == nullptr)
            data = stbi_load(from, &w, &h, &c, 0);
    }
    std::string from;
    std::string to;
    int w;
    int h;
    int c;
    unsigned char* data = nullptr;

};






#endif