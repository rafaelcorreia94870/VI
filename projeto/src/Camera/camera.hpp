//
//  camera.hpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 10/02/2023.
//

#ifndef camera_hpp
#define camera_hpp

#ifdef _WIN32
#include "../Rays/ray.hpp"

#elif __unix__ || __unix || __linux__ || __APPLE__
#include "ray.hpp"
#else
#error "Unsupported operating system"
#endif


// based on pbrt book, sec 6.1, pag. 356
class Camera {

public:
    Camera () {}
    ~Camera() {}
    virtual bool GenerateRay(const int x, const int y, Ray *r, const float *cam_jitter=NULL) {return false;};
    virtual void getResolution (int *_W, int *_H) {
        *_W=0; *_H=0;
    }
};

#endif /* camera_hpp */
