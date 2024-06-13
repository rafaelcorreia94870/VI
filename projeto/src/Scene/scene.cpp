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
#include <unordered_set>
#include <AreaLight.hpp>

using namespace tinyobj;

static void PrintInfo(const ObjReader myObj)
{
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
    for (; it_shape != shapes.end(); it_shape++)
    {
        // assume each face has 3 vertices
        std::cout << "Processing shape " << it_shape->name << std::endl;
        // iterate faces
        // assume each face has 3 vertices
        auto it_vertex = it_shape->mesh.indices.begin();
        for (; it_vertex != it_shape->mesh.indices.end();)
        {
            // process 3 vertices
            for (int v = 0; v < 3; v++)
            {
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
bool Scene::Load(const std::string &fname)
{

    tinyobj::ObjReaderConfig reader_config;
#ifdef _WIN32
    fs::path currentPath = fs::current_path();
    fs::path path = currentPath / ".." / "src" / "Scene" / "tinyobjloader" / "models";
    std::string pathStr = path.string();
    reader_config.mtl_search_path = pathStr;
#elif __unix__ || __unix || __linux__ || __APPLE__
    reader_config.mtl_search_path = "src/Scene/tinyobjloader/models/"; // Path to material files
#endif

    std::string inputfile = fname;
    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(inputfile, reader_config))
    {
        if (!reader.Error().empty())
        {
            std::cerr << "TinyObjReader: " << reader.Error();
        }
        exit(1);
    }

    if (!reader.Warning().empty())
    {
        std::cout << "TinyObjReader: " << reader.Warning();
    }

    // PrintInfo(reader);

    auto &attrib = reader.GetAttrib();
    auto &shapes = reader.GetShapes();
    auto &materials = reader.GetMaterials();

    numPrimitives = shapes.size();
    numBRDFs = materials.size();

    int repetidos = 0, unicos = 0;
    // Store unique vertices using a set
    std::vector<Point> unique_vertices;

    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++)
    {
        Primitive *p = new Primitive;
        size_t index_offset = 0;
        Mesh *m = new Mesh;
        m->numFaces = shapes[s].mesh.num_face_vertices.size();

        // Loop over faces(polygon)
        //std::cout << "\n----- [" << shapes[s].name << "] -----\n";
        BB *bb = new BB();
        for (size_t f = 0; f < m->numFaces; f++)
        {
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);
            //std::cout << "\nTriangle " << f + 1 << "/" << m->numFaces << "\n";

            Face *face = new Face;
            std::vector<Point> face_vertices; // Accumulator for geometriNnormal (face normal) calculations
            // Loop over vertices in the face->
            for (size_t v = 0; v < fv; v++)
            {
                // Access to vertex
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

                // Check if the vertex is already present
                bool found = false;
                size_t existing_index = 0;
                for (size_t i = 0; i < unique_vertices.size(); i++)
                {
                    if (attrib.vertices[3 * size_t(idx.vertex_index) + 0] == unique_vertices[i].X &&
                        attrib.vertices[3 * size_t(idx.vertex_index) + 1] == unique_vertices[i].Y &&
                        attrib.vertices[3 * size_t(idx.vertex_index) + 2] == unique_vertices[i].Z)
                    {
                        found = true;
                        existing_index = i;
                        break;
                    }
                }

                // Store unique vertices and add to face_vertices accumulator for normal calculations
                Point vert;
                if (!found)
                { // Unique vertex
                    vert.X = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
                    vert.Y = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
                    vert.Z = attrib.vertices[3 * size_t(idx.vertex_index) + 2];
                    // Store the index of the unique vertex
                    face->vert_ndx[v] = unique_vertices.size();
                    unique_vertices.push_back(vert);

                    unicos++; // debug
                }
                // If the vertex is not unique, store the index of the existing vertex
                else
                {
                    vert.X = unique_vertices[existing_index].X;
                    vert.Y = unique_vertices[existing_index].Y;
                    vert.Z = unique_vertices[existing_index].Z;
                    // Store the index of the existing vertex
                    face->vert_ndx[v] = existing_index;

                    repetidos++; // debug
                }

                // Print existing vertices
                /* std::cout << "  v[" << idx.vertex_index << "]: ("
                          << vert.X << ", " << vert.Y << ", " << vert.Z << ")\n"; */
                face_vertices.push_back(vert);
                bb->update(vert);

                // Check if `normal_index` is zero or positive. negative = no normal data
                if (idx.normal_index >= 0)
                {
                    face->hasShadingNormals = true;

                    face->vert_normals_ndx[0] = 3 * size_t(idx.normal_index) + 0;
                    face->vert_normals_ndx[1] = 3 * size_t(idx.normal_index) + 1;
                    face->vert_normals_ndx[2] = 3 * size_t(idx.normal_index) + 2;

                    Vector norm;
                    norm.X = attrib.normals[face->vert_normals_ndx[0]];
                    norm.Y = attrib.normals[face->vert_normals_ndx[1]];
                    norm.Z = attrib.normals[face->vert_normals_ndx[2]];

                    m->normals.push_back(norm);
                    m->numNormals++; // no fim: m->numNormals == m->numVertices
                }
                // Calculate face normal with vertices later
                else
                {
                    face->hasShadingNormals = false;
                }

                /* Check if `texcoord_index` is zero or positive.negative = no texcoord data
                if (idx.texcoord_index >= 0) {
                    tinyobj::real_t tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
                    tinyobj::real_t ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
                }
                */
            }

            // Now that all face vertices are available, calculate and store the normal.
            if (!face->hasShadingNormals)
            {
                // Access vertices from the temporary storage
                Point v1 = face_vertices[0];
                Point v2 = face_vertices[1];
                Point v3 = face_vertices[2];

                // Edge vector calculation
                Vector ex = v2.vec2point(v1); // Edge vector from v1 to v2
                Vector fx = v3.vec2point(v1); // Edge vector from v1 to v3

                // Calculate face normal using the cross product
                face->geoNormal = ex.cross(fx);

                // Normalize the face normal
                face->geoNormal.normalize();

                // Store in mesh normals vector
                m->normals.push_back(face->geoNormal);
                m->numNormals++;

                // std::cout << "      n[" << f << "]: (" << face->geoNormal.X << ", " << face->geoNormal.Y << ", " << face->geoNormal.Z << ")\n";
            }

            face_vertices.clear();
            m->faces.push_back(*face);
            index_offset += fv;
        }

        // Store unique vertices in the mesh
        m->vertices = unique_vertices;
        m->numVertices = unique_vertices.size();

        // Define the primitive's Geometry as the mesh
        m->bb = *bb;
        /* std::cout << " BB max:\n\t(" << m->bb.max.X << ", " << m->bb.max.Y << ", " << m->bb.max.Z << ")\n";
        std::cout << " BB min:\n\t(" << m->bb.min.X << ", " << m->bb.min.Y << ", " << m->bb.min.Z << ")\n"; */

        p->g = m;
        prims.push_back(p);

        // Define the mesh material (assuming all faces have the same material) as the first face material
        p->material_ndx = shapes[s].mesh.material_ids[0];
        //std::cout << "\nMaterial: " << p->material_ndx << "\n\n";
    }

    /* std::cout << "NÚMERO DE VÉRTICES UNICOS: " << unicos << "\n";
    std::cout << "NÚMERO DE VÉRTICES REPETIDOS: " << repetidos << "\n\n";
    std::cout << "---------------------------------------------------\n\n"; */

    // Loop over materials and add them to the BRDFs vector
    for (size_t i = 0; i < materials.size(); i++)
    {
        Phong *material = new Phong;
        RGB Ka, Kd, Ks, Kt;
        float Ns;

        Ka.R = materials[i].ambient[0];
        Ka.G = materials[i].ambient[1];
        Ka.B = materials[i].ambient[2];

        Kd.R = materials[i].diffuse[0];
        Kd.G = materials[i].diffuse[1];
        Kd.B = materials[i].diffuse[2];

        Ks.R = materials[i].specular[0];
        Ks.G = materials[i].specular[1];
        Ks.B = materials[i].specular[2];

        Kt.R = materials[i].transmittance[0];
        Kt.G = materials[i].transmittance[1];
        Kt.B = materials[i].transmittance[2];

        Ns = materials[i].shininess; // defaut = 1, if not defined in .mtl file

        // Assign mtl values to created BRDF
        material->Ka = Ka;
        material->Kd = Kd;
        material->Ks = Ks;
        material->Ns = Ns;

        /* if (i == 6)
        {
            std::cout << "Ka: (" << Ka.R << ", " << Ka.G << ", " << Ka.B << ")\n";
            std::cout << "Kd: (" << Kd.R << ", " << Kd.G << ", " << Kd.B << ")\n";
            std::cout << "Ks: (" << Ks.R << ", " << Ks.G << ", " << Ks.B << ")\n";
        } */

        BRDFs.push_back(material);
    }

    return true;
}

bool Scene::trace(Ray r, Intersection *isect)
{
    Intersection curr_isect;
    bool intersection = false;

    if (numPrimitives == 0)
        return false;

    // iterate over all primitives
    for (auto prim_itr = prims.begin(); prim_itr != prims.end(); prim_itr++)
    {
        if ((*prim_itr)->g->intersect(r, &curr_isect))
        {

            if (!intersection)
            { // first intersection
                intersection = true;
                *isect = curr_isect;
                isect->f = BRDFs[(*prim_itr)->material_ndx];
            }
            else if (curr_isect.depth < isect->depth)
            {
                *isect = curr_isect;
                isect->f = BRDFs[(*prim_itr)->material_ndx];
            }
        }
    }

    if (isect->pix_x != -1)
    {

        isect->isLight = false;
        for (auto l = lights.begin(); l != lights.end(); l++)
        {
            if ((*l)->type == AREA_LIGHT)
            {
                AreaLight *al = (AreaLight *)*l;
                if (al->gem->intersect(r, &curr_isect))
                {
                    if (!intersection)
                    { // first intersection
                        intersection = true;
                        *isect = curr_isect;
                        isect->isLight = true;
                        isect->Le = al->L();
                    }
                    else if (curr_isect.depth < isect->depth)
                    {
                        *isect = curr_isect;
                        isect->isLight = true;
                        isect->Le = al->L();
                    }
                }
            }
        }
    }

    return intersection;
}

// checks whether a point on a light source (distance maxL) is visible
bool Scene::visibility(Ray s, const float maxL)
{
    bool visible = true;
    Intersection curr_isect;

    if (numPrimitives == 0)
        return true;

    // iterate over all primitives while visible
    for (auto prim_itr = prims.begin(); prim_itr != prims.end() && visible; prim_itr++)
    {
        if ((*prim_itr)->g->intersect(s, &curr_isect))
        {
            if (curr_isect.depth < maxL)
            {
                visible = false;
            }
        }
    }
    return visible;
}
