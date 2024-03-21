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
    /*
    Point v1, v2, v3;
    Vector n;
    v1 = vertices[f.vert_ndx[0]];
    v2 = vertices[f.vert_ndx[1]];
    v3 = vertices[f.vert_ndx[2]];
    constexpr float epsilon = std::numeric_limits<float>::epsilon();

    Vector edge1 = v1.vec2point(v2);
    Vector edge2 = v1.vec2point(v3);
    Vector ray_cross_e2 = r.dir.cross(edge2);
    float det = edge1.dot(ray_cross_e2);

    if (det > -epsilon && det < epsilon)
        return false;    // This ray is parallel to this triangle.

    float inv_det = 1.0 / det;
    Vector s = v1.vec2point(r.o);
    float u = inv_det * s.dot(ray_cross_e2);

    if (u < 0 || u > 1)
        return false;

    Vector s_cross_e1 = s.cross(edge1);
    float v = inv_det * r.dir.dot(s_cross_e1);

    if (v < 0 || u + v > 1)
        return false;

    // At this stage we can compute t to find out where the intersection point is on the line.
    float t = inv_det * edge2.dot(s_cross_e1);

    if (t > epsilon) // ray intersection
    {
        isect->p = (r.o).operator+(r.dir).operator*(t);
        return true;
    }
    else // This means that there is a line intersection but not a ray intersection.
        return false;
        */
    
    Point v1, v2, v3;
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
    /*::cout
    << "  intersect Triangle (" 
    << f.vert_ndx[0] << ", "
    << f.vert_ndx[1] << ", "
    << f.vert_ndx[2] << ")" 
    << "\n";
    */
    return t.intersect(r,isect);
}

bool Mesh::intersect (Ray r, Intersection *isect) {
    if (r.pix_x == 63 && r.pix_y == 63) {
        std::cout << "R DIR: " << r.dir.X << " " << r.dir.Y << " " << r.dir.Z << "\n";
        std::cout << "R Origin: " << r.o.X << " " << r.o.Y << " " << r.o.Z << "\n";
        std::cout << "R invDir: " << r.invDir.X << " " << r.invDir.Y << " " << r.invDir.Z << "\n";
        std::cout << "R Dir: " << r.dir.X << " " << r.dir.Y << " " << r.dir.Z << "\n";
        std::cout << "R BB MAX: " << this->bb.max.X << " " << this->bb.max.Y << " " << this->bb.max.Z << "\n";
        std::cout << "R BB MIN: " << this->bb.min.X << " " << this->bb.min.Y << " " << this->bb.min.Z << "\n";

    }

    bool intersect = true, intersect_this_face;
    Intersection min_isect, curr_isect;
    float min_depth=MAXFLOAT;

    // intersect the ray with the mesh BB
    if (!bb.intersect(r)) {
        return false;
    }
    std::cout << "BB intersect sucess\n";

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
    //std::cout << intersect <<"\n";

    return intersect;
}
