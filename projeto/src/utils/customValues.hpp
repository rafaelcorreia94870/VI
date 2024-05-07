#ifndef CUSTOM_VALUES_HPP
#define CUSTOM_VALUES_HPP

#include <iostream>
#include <string>
#include <sstream>

#ifdef _WIN32
    #include "../utils/vector.hpp"
#elif __unix__ || __unix || __linux__ || __APPLE__
    #include "vector.hpp"
#else
    #error "Unsupported operating system"
#endif


void getCustomEyePoint(Point* Eye);
void getCustomAtPoint(Point* At);
void getCustomFov(float* fovW, int* W, int* H);
void getCustomFilename(std::string* userInput);
void getCustomSPP(int* spp);

void getCustomValues(Point *Eye, Point *At, float *fovW, int *W, int *H, int *spp, std::string *filename);
void defaultCornellBox(Point *Eye, Point *At, float *fovW, int *W, int *H, int *spp, std::string *filename);
void defaultTriangle(Point *Eye, Point *At, float *fovW, int *W, int *H, int *spp, std::string *filename);

#endif // CUSTOM_VALUES_HPP
