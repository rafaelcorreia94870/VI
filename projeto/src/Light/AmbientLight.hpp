//
//  AmbientLight.hpp
//  VI-RT-LPS
//
//  Created by Luis Paulo Santos on 14/03/2023.
//

#ifndef AmbientLight_hpp
#define AmbientLight_hpp

#include "light.hpp"

class AmbientLight: public Light {
public:
    RGB color;
    AmbientLight (RGB _color): color(_color) { type = AMBIENT_LIGHT; }
    ~AmbientLight () {}
    // return the Light RGB radiance for a given point : p
    RGB L (Point p) {return color;}
    // return the Light RGB radiance
    RGB L () {return color;}
    // return a point p and RGB radiance for a given probability pair prob[2]
    RGB Sample_L (float *prob, Point *p) {
        p = NULL;
        return color;
    }
};

#endif /* AmbientLight_hpp */
