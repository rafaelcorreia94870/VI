//
//  image.hpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 10/02/2023.
//

#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string>
#include <cstring>
#include <iostream>

#ifdef _WIN32
#include "../utils/RGB.hpp"
#elif __unix__ || __unix || __linux__ || __APPLE__
#include "RGB.hpp"
#else
#error "Unsupported operating system"
#endif

class Image
{
protected:
    RGB *imagePlane;
    int W, H;
    unsigned char *imageData; // Add this member to store converted data

public:
    Image() : W(0), H(0), imagePlane(nullptr), imageData(nullptr) {}
    Image(const int W, const int H) : W(W), H(H), imagePlane(new RGB[W * H]), imageData(new unsigned char[W * H * 3])
    {
        memset((void *)imagePlane, 0, W * H * sizeof(RGB)); // set image plane to 0
    }
    virtual     ~Image()
    {
        if (imagePlane != nullptr)
            delete[] imagePlane;
        if (imageData != nullptr)
            delete[] imageData;
    }
    bool set(int x, int y, const RGB &rgb)
    {
        if (x >= W || y >= H || x < 0 || y < 0)
            return false;
        imagePlane[y * W + x] = rgb;
        return true;
    }
    bool add(int x, int y, const RGB &rgb)
    {
        if (x >= W || y >= H || x < 0 || y < 0)
            return false;
        imagePlane[y * W + x] += rgb;
        return true;
    }
    const RGB *getData() const { return imagePlane; }
    bool Save(std::string filename) { return true; }

    // Convert image data to unsigned char format for OpenGL
    const unsigned char *getCharData()
    {
        unsigned char *imageData = new unsigned char[W * H * 3]; // Allocate memory for the image data
        for (int i = 0; i < W * H; ++i)
        {
            imageData[i * 3] = static_cast<unsigned char>(std::min(1.0f, imagePlane[i].R) * 255);
            imageData[i * 3 + 1] = static_cast<unsigned char>(std::min(1.0f, imagePlane[i].G) * 255);
            imageData[i * 3 + 2] = static_cast<unsigned char>(std::min(1.0f, imagePlane[i].B) * 255);
        }
        return imageData;
    }
};

#endif // IMAGE_HPP
