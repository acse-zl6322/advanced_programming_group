// A minimal example file showing how to include the stb image headers,
// and how to read and write image files using these headers.
//
// Compilation with (e.g.): g++-13 minimal.cpp -o minimal
//
// Written by T.M. Davison (2023)

#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

unsigned char* applyGrayScaleFilter(unsigned char*, const int&, const int&, int&);
unsigned char* applyHistogramEqualisation(unsigned char*, const int&, const int&, int&);
void rgbToHsvByPixel(const unsigned char, const unsigned char, const unsigned char, float&, float&, float&);
void hsvToRgbByPixel(const float, const float, const float, unsigned char&, unsigned char&, unsigned char&);


int main() {

    int w, h, c;
    unsigned char* data;

    // Read in image file
    data = stbi_load("..\\Images\\gracehopper.png", &w, &h, &c, 0);
    // data = stbi_load("..\\Output\\output.png", &w, &h, &c, 0);

    // Print image size to screen
    std::cout << "Image loaded with size " << w << " x " << h << " with " << c << " channel(s)." << std::endl;

    // int step = 4;
    // std::cout << int(*(data + step));
    
    // auto filtered = applyGrayScaleFilter(data, w, h, c);
    auto filtered = applyHistogramEqualisation(data, w, h, c);
    // Save image to new filename
    // int success = stbi_write_png("..\\GrayOutput\\output.png", w, h, c, filtered, 0);
    int success = stbi_write_png("..\\Output\\he_output_3.png", w, h, c, filtered, 0);
    
    // Deallocate memory
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

unsigned char* applyHistogramEqualisation(unsigned char* data, const int& w, const int& h, int& c){
    unsigned char* result = nullptr;
     if(1 == c) {
        int unormalized_cdf[256] = {0};
        result = new unsigned char[w * h];
        // frequency count
        for(int i=0; i<w*h; i++)
            unormalized_cdf[*(data + i)] += 1;
        // partial sum
        for(int i = 1; i < 256; i++)
            unormalized_cdf[i] += unormalized_cdf[i-1];
        for(int i=0; i< w*h; i++){
            unsigned char intensity = *(data + i);
            result[i] = 255 * unormalized_cdf[intensity] / w / h;
        }
     }else if(c >= 3){
        // If there are more than 3 channels, reduce it to 3 channels
        if(c > 3){
            unsigned char* reduced_data = new unsigned char[w * h * 3];
            for(int k=0; k < w*h; k++){
                reduced_data[k * 3] = data[k * c];
                reduced_data[k * 3 + 1] = data[k * c + 1];
                reduced_data[k* 3 + 2] = data[k * c + 2];
            }
            c = 3;
            data = reduced_data;
        }
        result = new unsigned char[w * h * c];
        float* hsvData = new float[w * h * c];
        for(int k= 0; k < w * h; k += c){
                float h, s, v;
                rgbToHsvByPixel(data[k*c], data[k*c + 1], data[k*c + 2], h, s, v);
                hsvData[k * 3] = h;
                hsvData[k * 3 + 1] = s;
                hsvData[k * 3 + 2] = v;
        }
            // Step 2: Perform histogram equalization on the value (V) channel
            // (Assuming 256 bins for simplicity)
            int* histogram = new int[256];
        for (int i = 0; i < w*h; i += c) {
                int value = static_cast<int>(hsvData[i + 2] * 255.0f);
                histogram[value]++;
        }
        float* cumulativeHistogram = new float[256];
        cumulativeHistogram[0] = histogram[0];
        for (int i = 1; i < 256; ++i) 
                cumulativeHistogram[i] = cumulativeHistogram[i - 1] + histogram[i];
        for (int i = 0; i < w*h; ++i) {
                float v = hsvData[i * 3 + 2];
                int newValue = static_cast<int>((cumulativeHistogram[static_cast<int>(v * 255.0f)] / w/ h) * 255.0f);
                hsvData[i * 3 + 2] = static_cast<float>(newValue) / 255.0f;
            }

        for (int i = 0; i < w*h; ++i) {
                float h = hsvData[i * 3];
                float s = hsvData[i * 3 + 1];
                float v = hsvData[i * 3 + 2];
                unsigned char r, g, b;
                hsvToRgbByPixel(h, s, v, r, g, b);
                result[i * 3] = r;
                result[i * 3 + 1] = g;
                result[i * 3 + 2] = b;
            }
     return result;
}}

void rgbToHsvByPixel(const unsigned char r, const unsigned char g, const unsigned char b, float &h, float &s, float &v){
    // Convert RGB values to range 0 to 1 by normalizing
    float normR = static_cast<float>(r) / 255.0f;
    float normG = static_cast<float>(g) / 255.0f;
    float normB = static_cast<float>(b) / 255.0f;

    // Find maximum and minimum RGB components
    float cmax = std::max(std::max(normR, normG), normB);
    float cmin = std::min(std::min(normR, normG), normB);
    float delta = cmax - cmin;

    // Compute value (V)
    v = cmax;

    // Compute saturation (S)
    if (cmax > 0.0f) {
        s = delta / cmax;
    } else {
        // If cmax is 0, set saturation to 0 to avoid division by zero
        s = 0.0f;
    }

    // Compute hue (H)
    if (delta > 0.0f) {
        if (cmax == normR) {
            h = 60.0f * fmodf(((normG - normB) / delta), 6.0f);
        } else if (cmax == normG) {
            h = 60.0f * (((normB - normR) / delta) + 2.0f);
        } else { // cmax == normB
            h = 60.0f * (((normR - normG) / delta) + 4.0f);
        }
    } else {
        // If delta is 0, hue is undefined, set it to 0
        h = 0.0f;
    }

    // Ensure hue is in the range [0, 360)
    if (h < 0.0f) {
        h += 360.0f;
    }
}

// Function to transform HSV to RGB for a single pixel
void hsvToRgbByPixel(const float h, const float s, const float v, unsigned char& r, unsigned char& g, unsigned char& b) {
    // Convert hue to sector index (0 to 5)
    float sector = h / 60.0f;
    int sectorIndex = static_cast<int>(floor(sector));
    float fractionalSector = sector - sectorIndex;

    // Calculate RGB values based on the sector
    float p = v * (1 - s);
    float q = v * (1 - s * fractionalSector);
    float t = v * (1 - s * (1 - fractionalSector));

    switch (sectorIndex) {
        case 0:
            r = static_cast<unsigned char>(v * 255);
            g = static_cast<unsigned char>(t * 255);
            b = static_cast<unsigned char>(p * 255);
            break;
        case 1:
            r = static_cast<unsigned char>(q * 255);
            g = static_cast<unsigned char>(v * 255);
            b = static_cast<unsigned char>(p * 255);
            break;
        case 2:
            r = static_cast<unsigned char>(p * 255);
            g = static_cast<unsigned char>(v * 255);
            b = static_cast<unsigned char>(t * 255);
            break;
        case 3:
            r = static_cast<unsigned char>(p * 255);
            g = static_cast<unsigned char>(q * 255);
            b = static_cast<unsigned char>(v * 255);
            break;
        case 4:
            r = static_cast<unsigned char>(t * 255);
            g = static_cast<unsigned char>(p * 255);
            b = static_cast<unsigned char>(v * 255);
            break;
        default: // case 5:
            r = static_cast<unsigned char>(v * 255);
            g = static_cast<unsigned char>(p * 255);
            b = static_cast<unsigned char>(q * 255);
            break;
    }
}
