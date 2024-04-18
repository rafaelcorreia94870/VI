//
//  mesh.cpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 05/02/2023.
//

#include "mesh.hpp"
#include "triangle.hpp"
#include <iostream>
#include "geometry.hpp"

// see pbrt book (3rd ed.), sec 3.6.2, pag 157
//
// Suggestion: use:
// // https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
// Moller Trumbore intersection algorithm

#ifdef _WIN32
    const float MAXFLOAT = std::numeric_limits<float>::max();
#endif

bool Mesh::TriangleIntersect (Ray r, Face f, Intersection *isect) {

    Point v1 = vertices[f.vert_ndx[0]];
    Point v2 = vertices[f.vert_ndx[1]];
    Point v3 = vertices[f.vert_ndx[2]];
    Vector n;

    if (!f.hasShadingNormals) {
        //std::cout << "  No Shading Normals\n";
        n = f.geoNormal;
    }
    else
    {
        //std::cout << "  Shading Normals\n";
        Vector n1 = normals[f.vert_normals_ndx[0]];
        Vector n2 = normals[f.vert_normals_ndx[1]];
        Vector n3 = normals[f.vert_normals_ndx[2]];
        n = n1 + n2 + n3;
        n.normalize();
    }
    Triangle t = Triangle(v1,v2,v3,n);

    return t.intersect(r,isect);
}

bool Mesh::FaceIntersect(Ray r, Face f, Intersection* isect) {

    if (!bb.intersect(r)) {
        return false;
    }
    Vector normal;
    if (!f.hasShadingNormals) {
        normal = f.geoNormal;
    }
    else {
        normal = ((this->normals[f.vert_normals_ndx[0]]+ this->normals[f.vert_normals_ndx[1]]+ this->normals[f.vert_normals_ndx[2]]));
        normal.normalize();
    }
    Vector wo = -1.f * r.dir;
    // make sure the normal points to the same side of the surface as wo
    Point v1 = vertices[f.vert_ndx[0]];
    Point v2 = vertices[f.vert_ndx[1]];
    Point v3 = vertices[f.vert_ndx[2]];

    normal = normal.Faceforward(wo);
    Vector edge1 = v1.vec2point(v2);
    Vector edge2 = v1.vec2point(v3);


    // Check whether the ray is parallel to the plan containing the triangle
    // The dot ptoduct between the ray direction and the triangle normal will be 0
    // also we require the ray to incide on the object on the same side
    // as the normal. i.e. dot(normal,r.dir) < EPSILON
    const float par = normal.dot(r.dir);
    if (par > (-EPSILON)) {
        return false;    // This ray is parallel to this triangle.
    }

    // now we want to solve
    // r.o - v0 = t * r.dir + u (v1-v0) + v (v2-v0)
    // there are 3 unknowns (t,u,v)
    // and 3 equations (for XX, YY, ZZ)

    Vector h, s, q;
    float a, ff, u, v;

    h = r.dir.cross(edge2);
    a = edge1.dot(h);
    ff = 1.0 / a;
    s = v1.vec2point(r.o);
    u = ff * s.dot(h);
    if (u < 0.0 || u > 1.0) {

        return false;
    }
    q = s.cross(edge1);
    v = ff * r.dir.dot(q);
    if (v < 0.0 || u + v > 1.0) {
        return false;
    }
    // At this stage we can compute t to find out where the intersection point is on the line.
    float t = ff * edge2.dot(q);

    if (t > EPSILON) // ray intersection
    {
        Point pHit = r.o + t * r.dir;
        // Fill Intersection data from triangle hit : pag 165
        isect->p = pHit;
        isect->gn = normal;
        isect->sn = normal;
        isect->wo = wo;
        isect->depth = t;
        isect->FaceID = -1;

        isect->pix_x = r.pix_x;
        isect->pix_y = r.pix_y;
        isect->isLight = false;
        return true;
    }
    else {// This means that there is a line intersection but not a ray intersection.
        return false;
    }
}

bool Mesh::intersect (Ray r, Intersection *isect) {

    bool intersect = true, intersect_this_face;
    Intersection min_isect, curr_isect;
    float min_depth=MAXFLOAT;

    // intersect the ray with the mesh BB
    if (!bb.intersect(r)) {
        return false;
    }

    // If it intersects then loop through the faces
    intersect = false;
    for (auto face_it=faces.begin() ; face_it != faces.end() ; face_it++) {
        intersect_this_face = FaceIntersect(r, *face_it, &curr_isect);
         
        if (!intersect_this_face) continue;
        intersect = true;
        if (curr_isect.depth < min_depth) {  // this is closer
            min_depth = curr_isect.depth;
            min_isect = curr_isect;
        }
    }

    if (intersect){
        *isect = min_isect;
    }

    return intersect;
}
