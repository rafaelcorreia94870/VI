//
//  perspective.cpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 10/02/2023.
//

#include "perspective.hpp"
#include <iostream>


void Perspective::PrintInfo() {
    std::cout <<"Eye X:" << this->Eye.X << " Eye Y:" << this->Eye.Y << " Eye Z:" << this->Eye.Z <<
        "\nAt X : " << this->At.X << " At Y :" << this->At.Y << " At Z : " << this->At.Z <<
        this->W << this->H << this-> fovW << this-> fovH;
}
Perspective::Perspective(const Point Eye, const Point At, const Vector Up, const int W, const int H, const float fovW, const float fovH) : Eye(Eye), At(At), Up(Up), W(W), H(H), fovW(fovW), fovH(fovH) {
    // compute camera 2 world transform
    Vector F, R, U;
    F = (this->Eye.vec2point(At));
    F.normalize();

    R = F.cross(Up);
    R.normalize();
    /*
    U = R.cross(F);
    U.normalize();
    this->Up = U;
    //n sei se devia tar a atualizar o Up com o U
    */
    U = this->Up;


    c2w[0][0] = R.X; c2w[0][1] = R.Y; c2w[0][2] = R.Z;
    c2w[1][0] = U.X; c2w[1][1] = U.Y; c2w[1][2] = U.Z;
    c2w[2][0] = F.X; c2w[2][1] = F.Y; c2w[2][2] = F.Z;

}

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

    r->pix_x = x;
    r->pix_y = y;


    r->o = this->Eye;
    return true;
}