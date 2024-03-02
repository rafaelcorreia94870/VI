//
//  Scene.cpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#include "scene.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#define TINYOBJLOADER_USE_MAPBOX_EARCUT

#ifdef _WIN32
#include "tinyobjloader/tiny_obj_loader.h"
#include "../Primitive/primitive.hpp"
#include "../Primitive/Geometry/mesh.hpp"
#include "../Primitive/BRDF/Phong.hpp"
#include <filesystem>
#include "../Primitive/Geometry/triangle.hpp"
namespace fs = std::filesystem;
#elif __unix__ || __unix || __linux__ || __APPLE__
#include "tiny_obj_loader.h"
#include "primitive.hpp"
#include "mesh.hpp"
#include "Phong.hpp"
#include "triangle.hpp"
#else
#error "Unsupported operating system"
#endif

#include <iostream>
#include <set>
#include <vector>

using namespace tinyobj;

static void PrintInfo (const ObjReader myObj) {
    const tinyobj::attrib_t attrib = myObj.GetAttrib();
    const std::vector<tinyobj::shape_t> shapes = myObj.GetShapes();
    const std::vector<tinyobj::material_t> materials = myObj.GetMaterials();
    std::cout << "# of vertices  : " << (attrib.vertices.size() / 3) << std::endl;
    std::cout << "# of normals   : " << (attrib.normals.size() / 3) << std::endl;
    std::cout << "# of texcoords : " << (attrib.texcoords.size() / 2)
              << std::endl;

    std::cout << "# of shapes    : " << shapes.size() << std::endl;
    std::cout << "# of materials : " << materials.size() << std::endl;
        
    // Iterate shapes
    auto it_shape = shapes.begin();
    for ( ; it_shape != shapes.end() ; it_shape++) {
        // assume each face has 3 vertices
        std::cout << "Processing shape " << it_shape->name << std::endl;
        // iterate faces
        // assume each face has 3 vertices
        auto it_vertex = it_shape->mesh.indices.begin();
        for ( ; it_vertex != it_shape->mesh.indices.end() ; ) {
            // process 3 vertices
            for (int v=0 ; v<3 ; v++) {
                std::cout << it_vertex->vertex_index;
                it_vertex++;
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
        
        printf("There are %lu material indexes\n", it_shape->mesh.material_ids.size());
    }
    
}

/*
 Use tiny load to load .obj scene descriptions
 https://github.com/tinyobjloader/tinyobjloader
 */

bool Scene::Load (const std::string &fname) {

#ifdef _WIN32
    std::string inputfile = fname;
    tinyobj::ObjReaderConfig reader_config;
    fs::path currentPath = fs::current_path();
    fs::path path = currentPath / ".." / "src" / "Scene" / "tinyobjloader" / "models";
    std::string pathStr = path.string();
    reader_config.mtl_search_path = pathStr;
#elif __unix__ || __unix || __linux__ || __APPLE__
    std::string inputfile = "/home/robert/aulas/4ano/2sem/VI/TP/VI/projeto/src/Scene/tinyobjloader/models/cornell_box.obj";
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = "/home/robert/aulas/4ano/2sem/VI/TP/VI/projeto/src/Scene/tinyobjloader/models"; // Path to material files
#endif

    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(inputfile, reader_config)) {
    if (!reader.Error().empty()) {
        std::cerr << "TinyObjReader: " << reader.Error();
    }
    exit(1);
    }

    if (!reader.Warning().empty()) {
    std::cout << "TinyObjReader: " << reader.Warning();
    }

    //PrintInfo(reader);

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();

    numPrimitives = shapes.size();
    numBRDFs = materials.size();

    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++) {
        Primitive p;
        size_t index_offset = 0;
        Mesh m;
        m.numFaces = shapes[s].mesh.num_face_vertices.size();
 

        // Loop over faces(polygon)
        std::cout << "----- [" << shapes[s].name << "] -----\n";
        for (size_t f = 0; f < m.numFaces; f++) {
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);
            std::cout << "\nTriangle " << f+1 << "/" << m.numFaces << "\n";
            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) {
                Face face;
                // access to vertex
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

                face.vert_ndx[0] = 3 * size_t(idx.vertex_index) + 0;
                face.vert_ndx[1] = 3 * size_t(idx.vertex_index) + 1;
                face.vert_ndx[2] = 3 * size_t(idx.vertex_index) + 2;

                Point vert;
                vert.X = attrib.vertices[face.vert_ndx[0]];
                vert.Y = attrib.vertices[face.vert_ndx[1]];
                vert.Z = attrib.vertices[face.vert_ndx[2]];

                m.vertices.push_back(vert);
                m.numVertices++;

                std::cout << "  v[" << idx.vertex_index << "]: (" << vert.X << ", " << vert.Y << ", " << vert.Z << ")\n";

                Vector norm;
                // Check if `normal_index` is zero or positive. negative = no normal data
                if (idx.normal_index >= 0) {
                    face.hasShadingNormals = true;

                    face.vert_normals_ndx[0] = 3 * size_t(idx.normal_index) + 0;
                    face.vert_normals_ndx[1] = 3 * size_t(idx.normal_index) + 1;
                    face.vert_normals_ndx[2] = 3 * size_t(idx.normal_index) + 2;

                    norm.X = attrib.normals[face.vert_normals_ndx[0]];
                    norm.Y = attrib.normals[face.vert_normals_ndx[1]];
                    norm.Z = attrib.normals[face.vert_normals_ndx[2]];
               
                    m.numNormals++;
                }
                // CALCULAR NORMAL A PARTIR DOS VERTICES
                else {

                }

                /* Check if `texcoord_index` is zero or positive.negative = no texcoord data
                if (idx.texcoord_index >= 0) {
                    tinyobj::real_t tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
                    tinyobj::real_t ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
                }
                */

                m.faces.push_back(face);
            }
            index_offset += fv;
        }
        // Define the primitive's Geometry as the mesh 
        p.g = &m;
        prims.push_back(&p);

        // Define the mesh material (assuming all faces have the same material) as the first face material
        p.material_ndx = shapes[s].mesh.material_ids[0];
        std::cout << "\nMaterial: " << p.material_ndx << "\n\n";
    }

    // Falta adicionar os materials ao vector BRDF 
    // Loop over materials
    for (size_t i = 0; i < materials.size(); i++) {
        std::cout << "Material " << i << ":\n";
        std::cout << "  name: " << materials[i].name << "\n";
        std::cout << "  ambient: " << materials[i].ambient[0] << ", " << materials[i].ambient[1] << ", " << materials[i].ambient[2] << "\n";
        std::cout << "  diffuse: " << materials[i].diffuse[0] << ", " << materials[i].diffuse[1] << ", " << materials[i].diffuse[2] << "\n";
        std::cout << "  specular: " << materials[i].specular[0] << ", " << materials[i].specular[1] << ", " << materials[i].specular[2] << "\n";
        std::cout << "-----------------\n";
    }

    return true;
}

bool Scene::trace (Ray r, Intersection *isect) {
    Intersection curr_isect;
    bool intersection = false;    
    
    if (numPrimitives==0) return false;
    
    // iterate over all primitives
    for (auto prim_itr = prims.begin() ; prim_itr != prims.end() ; prim_itr++) {
        if ((*prim_itr)->g->intersect(r, &curr_isect)) {
            if (!intersection) { // first intersection
                intersection = true;
                *isect = curr_isect;
                isect->f = BRDFs[(*prim_itr)->material_ndx];
            }
            else if (curr_isect.depth < isect->depth) {
                *isect = curr_isect;
                isect->f = BRDFs[(*prim_itr)->material_ndx];
            }
        }
    }
    return intersection;
}

// checks whether a point on a light source (distance maxL) is visible
bool Scene::visibility (Ray s, const float maxL) {
    bool visible = true;
    Intersection curr_isect;
    
    if (numPrimitives==0) return true;
    
    // iterate over all primitives while visible
    for (auto prim_itr = prims.begin() ; prim_itr != prims.end() && visible ; prim_itr++) {
        if ((*prim_itr)->g->intersect(s, &curr_isect)) {
            if (curr_isect.depth < maxL) {
                visible = false;
            }
        }
    }
    return visible;
}
