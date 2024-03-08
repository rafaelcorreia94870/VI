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
    Perspective (const Point Eye, const Point At, const Vector Up, const int W, const int H, const float fovW, const float fovH): Eye(Eye), At(At), Up(Up), W(W), H(H), fovW(fovW), fovH(fovH)  {
        // compute camera 2 world transform
        Vector F, R, U;
        F = (this->Eye.vec2point(At));
        F.normalize();
        
        R = F.cross(Up);
        R.normalize();

        U = R.cross(F);
        U.normalize();
        this->Up = U;
        //n sei se devia tar a atualizar o Up com o U


        c2w[0][0] = R.X; c2w[0][1] = R.Y; c2w[0][2] = R.Z;
        c2w[1][0] = U.X; c2w[1][1] = U.Y; c2w[1][2] = U.Z;
        c2w[2][0] = F.X; c2w[2][1] = F.Y; c2w[2][2] = F.Z;

    }
    bool GenerateRay(const int x, const int y, Ray *r, const float *cam_jitter=NULL);
    void getResolution (int *_W, int *_H) {*_W=W; *_H=H;}
};

#endif /* perspective_hpp */
