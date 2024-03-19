#pragma once
#include "Image.h"
#include <cmath> // For round()

class Filter {
public:
    // grayscale
    void convertToGrayscale(Image& image);

    // brightness
    void changeBrightness(Image& image, int value);

    // historgram
    unsigned char* applyHistogramEqualisation(Image& image);
    static void rgbToHsvByPixel(const unsigned char, const unsigned char, const unsigned char, float&, float&, float&);
    static void hsvToRgbByPixel(const float, const float, const float, unsigned char&, unsigned char&, unsigned char&);

    // threshold
    unsigned char* applyThreshold(Image& image, const unsigned char);


    // salt_pepper
    void addSaltAndPepperNoise(Image& image, float saltProbability, float pepperProbability);

    // blur
    // static std::vector<std::vector<float>> createGaussianKernel(int radius, float sigma);
    // void applyGaussianBlur(Image& image, int radius, float sigma);
    // void applyMedianBlur(Image& image, int kernelSize);
    // void applyBoxBlur(Image& image, int kernelSize);
    

    // // edge detection
    // enum class EdgeKernelType { Sobel, Prewitt, Scharr, Robert };
    // void edgeDetection(Image& image, const std::vector<EdgeKernelType>& kernels);


private:
    static const int SobelX[3][3];
    static const int SobelY[3][3];
    static const int PrewittX[3][3];
    static const int PrewittY[3][3];
    static const int ScharrX[3][3];
    static const int ScharrY[3][3];
    static const int RobertX[2][2];
    static const int RobertY[2][2];
};