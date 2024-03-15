//
//  geometry.hpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#ifndef geometry_hpp
#define geometry_hpp

#include "../BB.hpp"

#ifdef _WIN32
#include "../../Rays/ray.hpp"
#include "../../Rays/intersection.hpp"

#elif __unix__ || __unix || __linux__ || __APPLE__
#include "ray.hpp"
#include "intersection.hpp"
#else
#error "Unsupported operating system"
#endif

class Geometry {
public:
    Geometry () {}
    ~Geometry () {}
    // return True if r intersects this geometric primitive
    // returns data about intersection on isect
    virtual bool intersect(Ray r, Intersection* isect) { return false; }
    // geometric primitive bounding box
    BB bb;  // this is min={0.,0.,0.} , max={0.,0.,0.} due to the Point constructor
};

#endif /* geometry_hpp */
