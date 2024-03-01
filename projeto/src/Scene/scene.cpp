//
//  Scene.cpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#include "scene.hpp"

#define TINYOBJLOADER_IMPLEMENTATION

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
    ObjReader myObjReader;

    if (!myObjReader.ParseFromFile(fname)) {
        if (!myObjReader.Error().empty()) { std::cerr << "TinyObjReader: " << myObjReader.Error(); }
        return false;
    }
    
    //PrintInfo (myObjReader);


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

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();

    // Ou passar por argumento o apontador para uma Mesh à função Load??
    // Ou receber primitiva e adicionar a mesh à geometria
    Mesh mesh;

    // Povoar vertices da mesh
    for (size_t i = 0; i < attrib.vertices.size(); i += 3) {
        Point vertex;
        vertex.X = attrib.vertices[i + 0];
        vertex.Y = attrib.vertices[i + 1];
        vertex.Z = attrib.vertices[i + 2];
        mesh.vertices.push_back(vertex);
        mesh.numVertices++;
    }
    // Povoar normals da mesh
    for (size_t i = 0; i < attrib.normals.size(); i += 3) {
        Vector normal;
        normal.X = attrib.normals[i + 0];
        normal.Y = attrib.normals[i + 1];
        normal.Z = attrib.normals[i + 2];
        mesh.normals.push_back(normal);
        mesh.numNormals++;
    }

    //IMPRIMIR PARA VERFICAR A ESTRUTURA
    std::cout << "\nMesh vertices:";
    for (size_t index = 0; index < mesh.vertices.size(); ++index) {
        const auto& vertex = mesh.vertices[index];
        std::cout << "\n  v [" << index << "]: (" << vertex.X << ", " << vertex.Y << ", " << vertex.Z << ")";
    }

    std::cout << "\nMesh normals:";
    for (size_t index = 0; index < mesh.normals.size(); ++index) {
        const auto& normal = mesh.normals[index];
        std::cout << "\n  vn[" << index << "]: (" << normal.X << ", " << normal.Y << ", " << normal.Z << ")";
    }

    /* Print texture coordinates
    for (size_t i = 0; i < attrib.texcoords.size(); i += 2) {
        std::cout << "\n  vt[" << i / 2 << "]: (" << attrib.texcoords[i + 0] << ", " << attrib.texcoords[i + 1] << ")";
    }
    */

    // CONFIRMAR ESTA PARTE DAS FACES... ADICIONAR SHAPE PARA ASSOCIAR UM MATERIAL DIFERENTE A UM CONJ DE FACES???
    // TODO: Cena das normais, verificar se tem shading normals, senao calcular 
    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++) {
        // Loop over faces
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            Face face;

            // Loop over vertices in the face
            size_t fv = shapes[s].mesh.num_face_vertices[f];
            for (size_t v = 0; v < fv; v++) {
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

                // Store vertex index from Mesh class
                face.vert_ndx[v] = idx.vertex_index;

                // Optional: Store texture coordinate index (if applicable)
                // Modify according to your structure or requirements

                // Optional: Store normal index from Mesh class (if applicable)
                if (idx.normal_index >= 0) {
                    // Assuming mesh.normals is already populated
                    face.vert_normals_ndx[v] = idx.normal_index;
                }
            }

            // Optional: Calculate and store geometric normal (if needed)
            // ...

            // Optional: Check if there are per-vertex shading normals
            // face.hasShadingNormals = (shapes[s].mesh.smooth);

            // Optional: Update bounding box (if needed)
            // ...

            // Store the face in the mesh
            mesh.faces.push_back(face);
            mesh.numFaces++;

            index_offset += fv;
        }
    }

    std::cout << "\nMesh faces:";
    for (const auto& face : mesh.faces) {
        std::cout << "\n  Face: ";
        for (size_t v = 0; v < 3; v++) {
            // Imprimir indice dos vertices da face
            std::cout << face.vert_ndx[v];

            // Imprimir indices da normal (se tiver)
            if (v < 3 && face.vert_normals_ndx[v] >= 0) {
                std::cout << "/" << face.vert_normals_ndx[v];
            }

            std::cout << " ";
        }
    }

    std::cout << "\nmaterials:";
    // Loop over materials
    for (size_t m = 0; m < materials.size(); m++) {
        std::cout << "\n  material[" << m << "]: " << materials[m].name;
        // Print other material properties as needed
    }

    // your code here
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
