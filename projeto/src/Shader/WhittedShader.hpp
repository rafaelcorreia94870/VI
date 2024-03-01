//
//  AmbientShader.hpp
//  VI-RT-LPS
//
//  Created by Luis Paulo Santos on 14/03/2023.
//

#ifndef WhittedShader_hpp
#define WhittedShader_hpp

#include "shader.hpp"
#ifdef _WIN32
#include "../Primitive/BRDF/Phong.hpp"
#include "../Rays/ray.hpp"

#elif __unix__ || __unix || __linux__ || __APPLE__
#include "Phong.hpp"
#include "ray.hpp"
#else
#error "Unsupported operating system"
#endif

class WhittedShader: public Shader {
    RGB background;
    RGB directLighting (Intersection isect, Phong *f);
    RGB specularReflection (Intersection isect, Phong *f, int depth);
public:
    WhittedShader (Scene *scene, RGB bg): background(bg), Shader(scene) {}
    RGB shade (bool intersected, Intersection isect, int depth);
};

#endif /* AmbientShader_hpp */
