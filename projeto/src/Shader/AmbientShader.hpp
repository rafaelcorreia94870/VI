//
//  AmbientShader.hpp
//  VI-RT-LPS
//
//  Created by Luis Paulo Santos on 14/03/2023.
//

#ifndef AmbientShader_hpp
#define AmbientShader_hpp

#include "shader.hpp"
#include "../Primitive/BRDF/Phong.hpp"

class AmbientShader: public Shader {
    RGB background;
public:
    AmbientShader (Scene *scene, RGB bg): background(bg), Shader(scene) {}
    RGB shade (bool intersected, Intersection isect, int depth);
};

#endif /* AmbientShader_hpp */
