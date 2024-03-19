#ifndef COLOR_CORRECTION_H
#define COLOR_CORRECTION_H

unsigned char* applyHsvHistogramEqualisation(unsigned char*, const int&, const int&, int&);
unsigned char* applyHslHistogramEqualisation(unsigned char*, const int&, const int&, int&);
unsigned char* applyHslThreshold(unsigned char*, unsigned char, const int&, const int&, int&);
unsigned char* applyHsvThreshold(unsigned char*, unsigned char, const int&, const int&, int&);
void rgbToHsvByPixel(const unsigned char, const unsigned char, const unsigned char, float&, float&, float&);
void hsvToRgbByPixel(const float, const float, const float, unsigned char&, unsigned char&, unsigned char&);
void rgbToHslByPixel(const unsigned char, const unsigned char, const unsigned char, float&, float&, float&);
void hslToRgbByPixel(const float, const float, const float, unsigned char&, unsigned char&, unsigned char&);

#endif