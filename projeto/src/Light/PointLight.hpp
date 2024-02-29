//
//  PointLight.hpp
//  VI-RT-LPS
//
//  Created by Luis Paulo Santos on 06/04/2023.
//

#ifndef PointLight_hpp
#define PointLight_hpp

#include "light.hpp"

class PointLight: public Light {
public:
    RGB color;
    Point pos;
    PointLight (RGB _color, Point _pos): color(_color), pos(_pos) { type = POINT_LIGHT; }
    ~PointLight () {}
    // return the Light RGB radiance for a given point : p
    RGB L (Point p) {return color;}
    RGB L () {return color;}
    // return a point p and RGB radiance for a given probability pair prob[2]
    RGB Sample_L (float *prob, Point *p) {
        *p = pos;
        return color;
    }
};

#endif /* PointLight_hpp */
