//
//  AABB.hpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#ifndef BB_hpp
#define BB_hpp

#ifdef _WIN32
#include "../utils/vector.hpp"
#include "../Rays/ray.hpp"
#include "../Scene/tinyobjloader/tiny_obj_loader.h"
#elif __unix__ || __unix || __linux__ || __APPLE__
#include "vector.hpp"
#include "ray.hpp"
//not sure if this path is correct
#include "tiny_obj_loader.h"
#else
#error "Unsupported operating system"
#endif
#include <iostream>

typedef struct BB {
public:
    Point min, max;
    bool hasPoint = false;
    void update (Point p) {
        if (!hasPoint) {
            this->min = p;
            this->max = p;
            this->hasPoint = true;
        }
        else {
            if (p.X < min.X) min.X = p.X;
            else if (p.X > max.X) max.X = p.X;
            if (p.Y < min.Y) min.Y = p.Y;
            else if (p.Y > max.Y) max.Y = p.Y;
            if (p.Z < min.Z) min.Z = p.Z;
            else if (p.Z > max.Z) max.Z = p.Z;
        }
    }
    /*
     * I suggest you implement:
     *  bool intersect (Ray r) { }
     *
     * based on PBRT's 3rd ed. book , sec 3.1.2, pags 125..128 + 214,217,221
     *
     * or https://doi.org/10.1007/978-1-4842-7185-8_2
     *
     */
    bool intersect (Ray r) {
        Point P;
        Vector p0Dir = (r.o).vec2point(this->min);
        Vector p1Dir = (r.o).vec2point(this->max);
        //std::cout <<"MAX: " << " X: " << this->max.X << " Y: " << this->max.Y << " Z: " << this->max.Z << "\n";
        //std::cout << "MIN: " << " X: " << this->min.X << " Y: " << this->min.Y << " Z: " << this->min.Z << "\n";



        Vector invRayDir = r.invDir;
        //std::cout << invRayDir.X << invRayDir.Y << invRayDir.Z <<"\n";
        Vector tLower = p0Dir.cross(invRayDir);
        Vector tUpper = p1Dir.cross(invRayDir);
    
        // The four t-intervals (for x-/y-/z-slabs, and ray p(t))
        float tMins[4] = { fmin(tLower.X, tUpper.X), fmin(tLower.Y, tUpper.Y), fmin(tLower.Z, tUpper.Z), };
        float tMaxes[4] = { fmax(tLower.X, tUpper.X), fmax(tLower.Y, tUpper.Y), fmax(tLower.Z, tUpper.Z) };
        // Easy to remember: ``max of mins, and min of maxes''
        float tBoxMin = fmax(fmax(tMins[0], tMins[1]), tMins[2]);
        float tBoxMax = fmin(fmin(tMaxes[0], tMaxes[1]), tMaxes[2]);
        std::cout << "tBoxMin: " << tBoxMin << " tBoxMax:" << tBoxMax << "\n";
        return tBoxMin <= tBoxMax;
    }
} BB;

#endif /* AABB_hpp */
