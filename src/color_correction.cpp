#include "color_correction.h"
#include <string>
#include <cmath>

unsigned char* applyHistogramEqualisation(unsigned char* data, const int& w, const int& h, int& c) {
    if (c == 1) {
        // Grayscale image
        int unnormalized_cdf[256] = {0};

        // Frequency count
        for (int i = 0; i < w * h; i++)
            unnormalized_cdf[data[i]]++;

        // Partial sum
        for (int i = 1; i < 256; i++)
            unnormalized_cdf[i] += unnormalized_cdf[i - 1];

        // Normalize and map values
        for (int i = 0; i < w * h; i++)
            data[i] = 255 * unnormalized_cdf[data[i]] / (w * h);
    } else if (c >= 3) {
        if (c > 3) {
            // Reduce channels to 3
            unsigned char* reduced_data = new unsigned char[w * h * 3];
            for (int k = 0; k < w * h; k++) {
                for (int channel = 0; channel < 3; channel++) {
                    reduced_data[k * 3 + channel] = data[k * c + channel];
                }
            }
            c = 3;
            // delete[] data; // Free original data
            data = reduced_data;
        }

        // Convert RGB to HSV
        float* hsvData = new float[w * h * c];
        for (int k = 0; k < w * h; k++) {
            float h = 0.0f, s = 0.0f, v = 0.0f;
            rgbToHsvByPixel(data[c * k], data[c * k + 1], data[c * k + 2], h, s, v);
            hsvData[c * k] = h;
            hsvData[c * k + 1] = s;
            hsvData[c * k + 2] = v;
        }

        // Perform histogram equalization on V channel
        int unnormalized_cdf[256] = {0};
        for (int k = 0; k < w * h; k++) {
            int value = static_cast<int>(hsvData[k * c + 2] * 255.0f);
            unnormalized_cdf[value]++;
        }

        // Partial sum
        for (int i = 1; i < 256; i++)
            unnormalized_cdf[i] += unnormalized_cdf[i - 1];

        // Normalize and map values
        for (int k = 0; k < w * h; k++) {
            float original = hsvData[k * c + 2];
            hsvData[k * c + 2] = unnormalized_cdf[static_cast<int>(original * 255.0f)] / static_cast<float>(w * h);
        }

        // Convert HSV back to RGB
        for (int k = 0; k < w * h; k++) {
            unsigned char r = 0, g = 0, b = 0;
            hsvToRgbByPixel(hsvData[c * k], hsvData[c * k + 1], hsvData[c * k + 2], r, g, b);
            data[c * k] = r;
            data[c * k + 1] = g;
            data[c * k + 2] = b;
        }

        delete[] hsvData; // Free hsvData
    }
    return data;
}

unsigned char* applyThreshold(unsigned char* data, unsigned char threshold, const int& w, const int& h, int& c){
  if (c == 1) {
    // Normalize and map values
    for (int i = 0; i < w * h; i++)
        data[i] = (data[i] > threshold) ? 255 : 0;
  } else if (c >= 3) {
    if (c > 3) {
        // Reduce channels to 3
        unsigned char* reduced_data = new unsigned char[w * h * 3];
        for (int k = 0; k < w * h; k++) {
            for (int channel = 0; channel < 3; channel++) {
                reduced_data[k * 3 + channel] = data[k * c + channel];
            }
        }
        c = 3;
        data = reduced_data;
    }

    // Convert RGB to HSV
    float* hsvData = new float[w * h * c];
    for (int k = 0; k < w * h; k++) {
        float h = 0.0f, s = 0.0f, v = 0.0f;
        rgbToHsvByPixel(data[c * k], data[c * k + 1], data[c * k + 2], h, s, v);
        hsvData[c * k] = h;
        hsvData[c * k + 1] = 0;
        // L = (2 -S) /2 * V
        // float l = (2 - s) / 2 * v;
        hsvData[c * k + 2] = (v * 255 > threshold) ? 1 : 0;
    }

   
    // Convert HSV back to RGB
    for (int k = 0; k < w * h; k++) {
        unsigned char r = 0, g = 0, b = 0;
        hsvToRgbByPixel(hsvData[c * k], hsvData[c * k + 1], hsvData[c * k + 2], r, g, b);
        data[c * k] = r;
        data[c * k + 1] = g;
        data[c * k + 2] = b;
    }

    delete[] hsvData; // Free hsvData
  }
  return data;
  
}


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
