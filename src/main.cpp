// A minimal example file showing how to include the stb image headers,
// and how to read and write image files using these headers.
//
// Compilation with (e.g.): g++-13 minimal.cpp -o minimal
//
// Written by T.M. Davison (2023)

#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "color_correction.h"
#include "stb_image_write.h"

unsigned char* applyGrayScaleFilter(unsigned char*, const int&, const int&, int&);

int main() {

  int w, h, c;
  unsigned char *data;

  // Read in image file
  // data = stbi_load("..\\Images\\gracehopper.png", &w, &h, &c, 0);
  data = stbi_load("..\\Images\\tienshan.png", &w, &h, &c, 0);
  // data = stbi_load("..\\Output\\output.png", &w, &h, &c, 0);

  // Print image size to screen
  std::cout << "Image loaded with size " << w << " x " << h << " with " << c
            << " channel(s)." << std::endl;
//   auto oneChannel = applyGrayScaleFilter(data, w, h, c);
  // auto filtered = applyHslThreshold(data, 127, w, h, c);
  auto filtered = applyHslHistogramEqualisation(data, w, h, c);
  std::cout << "c now:" << c << std::endl;
  // int success = stbi_write_png("..\\Output\\4-threshold\\vh_ct_801.png", w, h, c, filtered, 0);
  int success = stbi_write_png("..\\Output\\hsl.png", w, h, c, filtered, 0);
  std::cout << "Image saved to file: " << success << std::endl;
  stbi_image_free(data);

  return 0;
}

unsigned char* applyGrayScaleFilter(unsigned char* data, const int& w, const int& h, int&c){
    unsigned char* filtered = new unsigned char[w * h];
    for(int i=0; i< w * h; i++)
        filtered[i] = 0.2126 * data[i * c] + 0.7152 * data[i * c + 1] + 0.0722 * data[i * c + 2];
    c = 1;
    return filtered;
}