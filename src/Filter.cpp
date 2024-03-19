#include "Filter.h"
#include <vector>
#include <random> // For random number generation


// Kernel Definitions
const int Filter::SobelX[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
const int Filter::SobelY[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
const int Filter::PrewittX[3][3] = {{-1, 0, 1}, {-1, 0, 1}, {-1, 0, 1}};
const int Filter::PrewittY[3][3] = {{-1, -1, -1}, {0, 0, 0}, {1, 1, 1}};
const int Filter::ScharrX[3][3] = {{-3, 0, 3}, {-10, 0, 10}, {-3, 0, 3}};
const int Filter::ScharrY[3][3] = {{-3, -10, -3}, {0, 0, 0}, {3, 10, 3}};
const int Filter::RobertX[2][2] = {{1, 0}, {0,-1}};
const int Filter::RobertY[2][2] = {{0, 1}, {-1,0}};

void Filter::convertToGrayscale(Image& image) {
    if (image.channels < 3) {
        std::cerr << "Image is already grayscale." << std::endl;
        return;
    }

    size_t dataSize = image.w * image.h;
    auto grayscaleData = std::make_unique<unsigned char[]>(dataSize);
    for (int i = 0; i < image.h; ++i) {
        for (int j = 0; j < image.w; ++j) {
            int index = (i * image.w + j) * image.channels;
            unsigned char grayscaleValue = static_cast<unsigned char>(
                0.2126 * image.data.get()[index] +
                0.7152 * image.data.get()[index + 1] +
                0.0722 * image.data.get()[index + 2]);
            grayscaleData[i * image.w + j] = grayscaleValue;
        }
    }

    // Replace the original image data with the grayscale data
    image.data = std::move(grayscaleData);
    image.channels = 1; // Update the channel count
}

void Filter::changeBrightness(Image& image, int value) {
    for (int i = 0; i < image.h; ++i) {
        for (int j = 0; j < image.w; ++j) {
            for (int k = 0; k < image.channels; ++k) { 
                int index = (i * image.w + j) * image.channels + k;
                int adjustedValue = static_cast<int>(image.data.get()[index]) + value;
                // Ensure new value is within [0,255]
                image.data.get()[index] = static_cast<unsigned char>(std::max(0, std::min(255, adjustedValue)));
            }
        }
    }
}

unsigned char* Filter::applyHistogramEqualisation(Image& image) {

    unsigned char* data = image.data.get();
    int h = image.h;
    int w = image.w;
    int c = image.channels;

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
        // image.data.reset(data);
    }
    return data;
}

void Filter::rgbToHsvByPixel(unsigned char r, unsigned char g, unsigned char b, float& h, float& s, float& v) {
    // Implementation...
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

void Filter::hsvToRgbByPixel(float h, float s, float v, unsigned char& r, unsigned char& g, unsigned char& b) {
    // Implementation...
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


unsigned char* Filter::applyThreshold(Image& image, unsigned char threshold) {
    unsigned char* data = image.data.get();
    int h = image.h;
    int w = image.w;
    int c = image.channels;

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
}




void Filter::addSaltAndPepperNoise(Image& image, float saltProbability, float pepperProbability) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0, 1.0);
    unsigned char* data = image.data.get();

    for (int y = 0; y < image.h; ++y) {
        for (int x = 0; x < image.w; ++x) {
            float random_val = dis(gen);
            if (random_val < saltProbability) {
                for (int ch = 0; ch < image.channels; ++ch) {
                    data[(y * image.w + x) * image.channels + ch] = 255;
                }
            } else if (random_val > 1 - pepperProbability) {
                for (int ch = 0; ch < image.channels; ++ch) {
                    data[(y * image.w + x) * image.channels + ch] = 0;
                }
            }
        }
    }
}



// std::vector<std::vector<float>> Filter::createGaussianKernel(int radius, float sigma) {
//     int size = 2 * radius + 1; // Size of the kernel
//     std::vector<std::vector<float>> kernel(size, std::vector<float>(size));
//     float sum = 0.0; // Used for normalization later

//     // Compute the Gaussian kernel
//     for (int i = -radius; i <= radius; i++) {
//         for (int j = -radius; j <= radius; j++) {
//             float exponent = -(i*i + j*j) / (2 * sigma * sigma);
//             kernel[i + radius][j + radius] = std::exp(exponent);
//             sum += kernel[i + radius][j + radius];
//         }
//     }

//     // Normalize the Gaussian kernel to ensure the sum of all elements is 1
//     for (int i = 0; i < size; i++) {
//         for (int j = 0; j < size; j++) {
//             kernel[i][j] /= sum;
//         }
//     }

//     return kernel;
// }




// void Filter::edgeDetection(Image& image, const std::vector<EdgeKernelType>& kernels) {
//     // Move the implementation from edge_detection.cpp here
//     // Adapt it to work within the Filter class and with the Image class
//     unsigned char* inputImage = image.data.get();
//     unsigned char* outputImage = new unsigned char[image.w * image.h];

//     for (const auto& kernelType : kernels) {
//         if (kernelType == EdgeKernelType::Robert) {
//             std::memcpy(outputImage, inputImage, image.w * image.h * sizeof(unsigned char));

//             for (int y = 0; y < image.h - 1; ++y) {
//                 for (int x = 0; x < image.w - 1; ++x) {
//                     int gx = 0, gy = 0;
//                     for (int i = 0; i <= 1; ++i) {
//                         for (int j = 0; j <= 1; ++j) {
//                             int pixelIndex = (y + i) * image.w + (x + j);
//                             gx += outputImage[pixelIndex] * RobertX[i][j];
//                             gy += outputImage[pixelIndex] * RobertY[i][j];
//                         }
//                     }
//                     int magnitude = static_cast<int>(sqrt(gx * gx + gy * gy));
//                     inputImage[y * image.w + x] = static_cast<unsigned char>(magnitude > 255 ? 255 : magnitude);
//                 }
//             }
//         } else {
//             const int (*kernelX)[3][3];
//             const int (*kernelY)[3][3];
        

//             switch (kernelType) {
//                 case EdgeKernelType::Sobel:
//                     kernelX = &SobelX;
//                     kernelY = &SobelY;
//                     break;
//                 case EdgeKernelType::Prewitt:
//                     kernelX = &PrewittX;
//                     kernelY = &PrewittY;
//                     break;
//                 case EdgeKernelType::Scharr:
//                     kernelX = &ScharrX;
//                     kernelY = &ScharrY;
//                     break;
//                 default:
//                     break;
//             }

//             std::memcpy(outputImage, inputImage, image.w * image.h * sizeof(unsigned char)); // Copy input to output for processing

//             for (int y = 1; y < image.h - 1; ++y) {
//                 for (int x = 1; x < image.w - 1; ++x) {
//                     int gx = 0, gy = 0;
//                     for (int i = -1; i <= 1; ++i) {
//                         for (int j = -1; j <= 1; ++j) {
//                             int pixelIndex = ((y + i) * image.w + (x + j));
//                             gx += outputImage[pixelIndex] * (*kernelX)[i + 1][j + 1];
//                             gy += outputImage[pixelIndex] * (*kernelY)[i + 1][j + 1];
//                         }
//                     }
//                     int magnitude = static_cast<int>(sqrt(gx * gx + gy * gy));
//                     inputImage[y * image.w + x] = static_cast<unsigned char>(magnitude);
//                 }
//             }
//             }
//     }

//     delete[] outputImage;
// }


