//
//  Phong.hpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 07/02/2023.
//

#ifndef Phong_hpp
#define Phong_hpp

#ifdef _WIN32
#include "../../utils/RGB.hpp"

#elif __unix__ || __unix || __linux__ || __APPLE__
#include "RGB.hpp"
#else
#error "Unsupported operating system"
#endif

#include "BRDF.hpp"

class Phong: public BRDF {
public:
    RGB Ka, Kd, Ks, Kt;
    float Ns;
};

#endif /* Phong_hpp */
