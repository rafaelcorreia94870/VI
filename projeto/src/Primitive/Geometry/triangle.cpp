//
//  mesh.cpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 05/02/2023.
//

#include "triangle.hpp"
#ifdef _WIN32
#include "../BB.hpp"

#elif __unix__ || __unix || __linux__ || __APPLE__
#include "BB.hpp"
#else
#error "Unsupported operating system"
#endif
#include <iostream>


// https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
// Moller Trumbore intersection algorithm
bool Triangle::intersect(Ray r, Intersection *isect) {

    if (!bb.intersect(r)) {
        return false;
    }

    // Check whether the ray is parallel to the plan containing the triangle
    // The dot ptoduct between the ray direction and the triangle normal will be 0
    // also we require the ray to incide on the object on the same side
    // as the normal. i.e. dot(normal,r.dir) < EPSILON
    normal = normal.Faceforward(-1.f*r.dir);
    const float par = normal.dot(r.dir);
    if (par>(-EPSILON)) {
        return false;    // This ray is parallel to this triangle.
    }

    // now we want to solve
    // r.o - v0 = t * r.dir + u (v1-v0) + v (v2-v0)
    // there are 3 unknowns (t,u,v)
    // and 3 equations (for XX, YY, ZZ)
    
    Vector h, s, q;
    float a,ff,u,v;

    h = r.dir.cross(this->edge2);
    a = edge1.dot(h);
    ff = 1.0/a;
    s = v1.vec2point(r.o);
    u = ff * s.dot(h);
    if (u < 0.0 || u > 1.0) {

        return false;
    }
    q = s.cross(this->edge1);
    v = ff * r.dir.dot(q);
    if (v < 0.0 || u + v > 1.0) {
        return false;
    }
    // At this stage we can compute t to find out where the intersection point is on the line.
    float t = ff * this->edge2.dot(q);
    
    if (t > EPSILON) // ray intersection
    {
        Point pHit = r.o + t* r.dir;
        
        // Fill Intersection data from triangle hit : pag 165
        Vector wo = -1.f * r.dir;
        // make sure the normal points to the same side of the surface as wo
        Vector for_normal = normal.Faceforward(wo);
           
        isect->p = pHit;
        isect->gn = for_normal;
        isect->sn = for_normal;
        isect->wo = wo;
        isect->depth = t;
        isect->FaceID = -1;
        
        isect->pix_x = r.pix_x;
        isect->pix_y = r.pix_y;
        isect->isLight = false;
        return true;
    }
    else  {// This means that there is a line intersection but not a ray intersection.
        return false;
    }
}

bool Triangle::isInside(Point p) {
    /* Calculate area of this triangle ABC */
    float A = area ();
  
    /* Calculate area of triangle p v1 v2 */
    float A1 = points_area (p, v1, v2);
    /* Calculate area of triangle p v2 v3 */
    float A2 = points_area (p, v2, v3);
    /* Calculate area of triangle p v3 v1 */
    float A3 = points_area (p, v3, v1);
    
   /* Check if sum of A1, A2 and A3 is same as A */
   return (A == A1 + A2 + A3);
}
