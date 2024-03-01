//
//  primitive.hpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#ifndef primitive_hpp
#define primitive_hpp

#ifdef _WIN32
#include "Geometry/geometry.hpp"
#include "BRDF/BRDF.hpp"

#elif __unix__ || __unix || __linux__ || __APPLE__
#include "geometry.hpp"
#include "BRDF.hpp"
#else
#error "Unsupported operating system"
#endif

typedef struct Primitive {
    Geometry *g;
    int material_ndx;
} Primitive;

#endif /* primitive_hpp */
