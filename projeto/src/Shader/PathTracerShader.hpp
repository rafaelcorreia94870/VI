//
//  AmbientShader.hpp
//  VI-RT-LPS
//
//  Created by Luis Paulo Santos on 14/03/2023.
//

#ifndef PathTracerShader_hpp
#define PathTracerShader_hpp

#include "shader.hpp"
#ifdef _WIN32
#include "../Primitive/BRDF/Phong.hpp"
#include "../Rays/ray.hpp"
#include "../Light/AreaLight.hpp"

#elif __unix__ || __unix || __linux__ || __APPLE__
#include "Phong.hpp"
#include "ray.hpp"
#include "AreaLight.hpp"
#else
#error "Unsupported operating system"
#endif

class PathTracerShader : public Shader
{
    bool russianRoulette;
    RGB background;
    RGB directLighting(Intersection isect, Phong *f);
    RGB specularReflection(Intersection isect, Phong *f, int depth);
    RGB diffuseReflection(Intersection isect, Phong *f, int depth);
    float continue_p;
    int MAX_DEPTH;

public:
    PathTracerShader(Scene *scene, RGB bg, bool rr) : background(bg), Shader(scene)
    {
        continue_p = 0.5f;
        MAX_DEPTH = 2;
        russianRoulette = rr;
    }
    RGB shade(bool intersected, Intersection isect, int depth);
};

#endif /* DistributedShader_hpp */
