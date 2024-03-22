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
        intersect_this_face = TriangleIntersect(r, *face_it, &curr_isect);
         
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
