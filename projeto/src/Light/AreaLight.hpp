//
//  PointLight.hpp
//  VI-RT-LPS
//
//  Created by Luis Paulo Santos on 06/04/2023.
//

#ifndef AreaLight_hpp
#define AreaLight_hpp

#include "light.hpp"
#include <math.h>

#ifdef _WIN32
#include "../Primitive/Geometry/triangle.hpp"


#elif __unix__ || __unix || __linux__ || __APPLE__
#include "triangle.hpp"
#else
#error "Unsupported operating system"
#endif

class AreaLight: public Light {
public:
    RGB intensity, power;
    Triangle *gem;
    float pdf;
    AreaLight (RGB _power, Point _v1, Point _v2, Point _v3, Vector _n): power(_power) {
        type = AREA_LIGHT;
        gem = new Triangle (_v1, _v2, _v3, _n);
        pdf = 1.f/gem->area();  // for uniform sampling over the area
        intensity = _power * pdf;
    }
    ~AreaLight () {delete gem;}
    // return the Light RGB radiance for a given point : p
    RGB L (Point p) {return power;}
    RGB L () {return power;}
    // return a point p, RGB radiance and pdf given a pair of random number in [0..[
    RGB Sample_L (float *r, Point *p, float &_pdf) {
        // sample point as described in the "Gloabl illumination Compendium", page 12, item 18
 
        const float sqrt_r0 = sqrtf(r[0]);
        const float alpha = 1.f - sqrt_r0;
        const float beta = (1.f - r[1]) * sqrt_r0;
        const float gamma = r[1] * sqrt_r0;
        p->X = alpha * gem->v1.X + beta * gem->v2.X + gamma * gem->v3.X;
        p->Y = alpha * gem->v1.Y + beta * gem->v2.Y + gamma * gem->v3.Y;
        p->Z = alpha * gem->v1.Z + beta * gem->v2.Z + gamma * gem->v3.Z;
        _pdf = pdf;
        return intensity;

    }
};

#endif /* AreaLight_hpp */
