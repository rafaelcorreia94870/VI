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
#include "../Scene/tinyobjloader/examples/viewer/viewer.cc"

#elif __unix__ || __unix || __linux__ || __APPLE__
#include "vector.hpp"
#include "ray.hpp"
//not sure if this path is correct
#include "tiny_obj_loader.h"
#include "viewer.cc"
#else
#error "Unsupported operating system"
#endif

typedef struct BB {
public:
    Point min, max;
    void update (Point p) {
        if (p.X < min.X) min.X = p.X;
        else if (p.X > max.X) max.X = p.X;
        if (p.Y < min.Y) min.Y = p.Y;
        else if (p.Y > max.Y) max.Y = p.Y;
        if (p.Z < min.Z) min.Z = p.Z;
        else if (p.Z > max.Z) max.Z = p.Z;
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
        Vector p0Dir = (r.o).vec2point(this->min);
        Vector p1Dir = (r.o).vec2point(this->max);
        Vector invRayDir = r.invDir;
        Vector tLower = p0Dir.cross(invRayDir);
        Vector tUpper = p1Dir.cross(invRayDir);

        // The four t-intervals (for x-/y-/z-slabs, and ray p(t))
        float tMins[4] = { std::min(tLower.X, tUpper.X), std::min(tLower.Y, tUpper.Y), std::min(tLower.Z, tUpper.Z), r.rayTmin };
        float tMaxes[4] = { std::max(tLower.X, tUpper.X), std::max(tLower.Y, tUpper.Y), std::max(tLower.Z, tUpper.Z), r.rayTmax };
        // Easy to remember: ``max of mins, and min of maxes''
        float tBoxMin = std::max({ tMins[0], tMins[1], tMins[2], tMins[3] });
        float tBoxMax = std::min({ tMaxes[0], tMaxes[1], tMaxes[2], tMaxes[3] });
        return tBoxMin <= tBoxMax;
        return true; 
    }
} BB;

#endif /* AABB_hpp */
