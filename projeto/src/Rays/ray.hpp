//
//  Ray.hpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#ifndef Ray_hpp
#define Ray_hpp

#ifdef _WIN32
#include "../utils/vector.hpp"

#elif __unix__ || __unix || __linux__ || __APPLE__
#include "vector.hpp"
#else
#error "Unsupported operating system"
#endif
#include <cfloat>
#include <iostream>

const float EPSILON=1e-3;

class Ray {
public:
    Point o; // ray origin
    Vector dir; // ray direction
    int FaceID;  // ID of the face where the origin lays in
    Vector invDir;  // ray direction reciprocal for intersections
    int pix_x, pix_y;
    float rayTmin, rayTmax; //Start and End of Ray (to limit calculations)
    /*
    RGB ():R(0.),G(0.),B(0.) {}
    RGB (float r, float g, float b):R(r),G(g),B(b) {}
    RGB (float *rgb):R(rgb[0]),G(rgb[1]),B(rgb[2]) {}
    ~RGB () {}
*/
    Ray() : o(Point(0, 0, 0)), dir(Vector(0, 0, 0)), FaceID(0), invDir(Vector(0, 0, 0)), pix_x(0), pix_y(0), rayTmin(0.0), rayTmax(FLT_MAX) {}
    Ray(Point o, Vector d) : o(o), dir(d), FaceID(0), invDir(Vector(0, 0, 0)), pix_x(0), pix_y(0), rayTmin(0.0), rayTmax(FLT_MAX) {}
    Ray(Point o, Vector d, int FaceID, Vector invDir, int pix_x = 0, int pix_y = 0, float rayTmin = 0.0, float rayTmax = FLT_MAX)
        : o(o), dir(d), FaceID(FaceID), invDir(invDir), pix_x(pix_x), pix_y(pix_y), rayTmin(rayTmin), rayTmax(rayTmax) {}
    ~Ray() {}
    void adjustOrigin (Vector normal) {
        Vector offset = EPSILON * normal;
        if (dir.dot(normal) < 0)
            offset = -1.f * offset;
        o.X = o.X + offset.X;
        o.Y = o.Y + offset.Y;
        o.Z = o.Z + offset.Z;
    }
};

#endif /* Ray_hpp */
