#pragma once
#include <iostream>
//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
//#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <memory>


struct Image
{
    std::shared_ptr<unsigned char[]> data;
    size_t  size;
    int w;
    int h;
    int channels;

    // Constructors and destructor
    Image(const std::string& filePath);
    Image(int _w, int _h, int _c);

    // Equivalency operator

    bool Read(const std::string& filePath);
    bool Write(const std::string& filePath);
    bool convertToRGB();
    
    void describe() const;

    friend void swap(Image& first, Image& second) noexcept {
        using std::swap;
        swap(first.w, second.w);
        swap(first.h, second.h);
        swap(first.channels, second.channels);
        swap(first.data, second.data);
        swap(first.size, second.size);
    }
private:
    bool newAllocation;
};
