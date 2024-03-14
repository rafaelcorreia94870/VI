//
//  perspective.hpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 10/02/2023.
//

#ifndef perspective_hpp
#define perspective_hpp

#include "camera.hpp"

#ifdef _WIN32
#include "../Rays/ray.hpp"

#elif __unix__ || __unix || __linux__ || __APPLE__
#include "ray.hpp"
#else
#error "Unsupported operating system"
#endif

class Perspective: public Camera {
    Point Eye, At;
    Vector Up;
    float fovW, fovH;
    int W, H;
    float c2w[3][3];  // camera 2 world transform
public:
    Perspective(const Point Eye, const Point At, const Vector Up, const int W, const int H, const float fovW, const float fovH);
    bool GenerateRay(const int x, const int y, Ray *r, const float *cam_jitter=NULL);
    void getResolution (int *_W, int *_H) {*_W=W; *_H=H;}
};

#endif /* perspective_hpp */
