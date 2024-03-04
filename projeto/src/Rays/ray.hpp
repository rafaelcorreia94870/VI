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

const float EPSILON=1e-3;

class Ray {
public:
    Point o; // ray origin
    Vector dir; // ray direction
    int FaceID;  // ID of the face where the origin lays in
    Vector invDir;  // ray direction reciprocal for intersections
    int pix_x, pix_y;
    float rayTmin, rayTmax; //Start and End of Ray (to limit calculations)
    Ray () {}
    Ray (Point o, Vector d): o(o),dir(d) {}
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
