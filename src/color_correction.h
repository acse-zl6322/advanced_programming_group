#ifndef COLOR_CORRECTION_H
#define COLOR_CORRECTION_H

unsigned char* applyHistogramEqualisation(unsigned char*, const int&, const int&, int&);
unsigned char* applyThreshold(unsigned char*, unsigned char, const int&, const int&, int&);
void rgbToHsvByPixel(const unsigned char, const unsigned char, const unsigned char, float&, float&, float&);
void hsvToRgbByPixel(const float, const float, const float, unsigned char&, unsigned char&, unsigned char&);


#endif