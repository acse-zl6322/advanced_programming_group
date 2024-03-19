#include "Image.h"

Image::Image(const std::string& filePath){
    if(Read(filePath)){
        //std::cerr<< "Read" << filename <<std::endl;
        newAllocation = false;
    } else{
        std::cerr<< "Failed to read" << filePath << std::endl;
    }
}

Image::Image(int _w, int _h, int _c) : w(_w), h(_h), channels(_c), size(_w*_h*_c), data(new unsigned char[_w*_h*_c]) {}


bool Image::Read(const std::string& filePath) {
    int width, height, channels;
    unsigned char* raw_data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
    if (!raw_data) return false;

    w = width;
    h = height;
    this->channels = channels;
    size = w * h * channels;
    data.reset(raw_data);

    return true;
}

bool Image::Write(const std::string& filePath) {
    if (data != nullptr){
        std::cerr << "Writing to file" << std::endl;
         return stbi_write_png(filePath.c_str(), w, h, channels, data.get(), w * channels) != 0;
    } 
    else {
        std::cerr << "Error image data is nullptr " << std::endl;
        return false;
    }
    
   
}

void Image::describe() const {
    std::cout << "Image with size " << w << " x " << h << " with " << channels << " channel(s)." << std::endl;
}

