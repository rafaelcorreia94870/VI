//
//  perspective.cpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 10/02/2023.
//

#include "perspective.hpp"

bool Perspective::GenerateRay(const int x, const int y, Ray *r, const float *cam_jitter) {
    Vector result;
    Vector xy1 = {static_cast<float>(x),static_cast<float>(y),1};
    Vector RUF[3] = { Vector(c2w[0][1],c2w[0][2],c2w[0][2]),
                      Vector(c2w[1][1],c2w[1][2],c2w[1][2]), 
                      Vector(c2w[2][1],c2w[2][2],c2w[2][2])};
    result.X = RUF[0].dot(xy1);
    result.Y = RUF[1].dot(xy1);
    result.Z = RUF[2].dot(xy1);

    r->dir = result;


    r->o = this->Eye;
    return true;
}
