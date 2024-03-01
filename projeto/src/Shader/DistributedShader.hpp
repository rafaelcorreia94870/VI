//
//  AmbientShader.hpp
//  VI-RT-LPS
//
//  Created by Luis Paulo Santos on 14/03/2023.
//

#ifndef DistributedShader_hpp
#define DistributedShader_hpp

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

class DistributedShader: public Shader {
    RGB background;
    RGB directLighting (Intersection isect, Phong *f);
    RGB specularReflection (Intersection isect, Phong *f, int depth);
public:
    DistributedShader (Scene *scene, RGB bg): background(bg), Shader(scene) {}
    RGB shade (bool intersected, Intersection isect, int depth);
};

#endif /* DistributedShader_hpp */
