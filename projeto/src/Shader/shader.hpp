//
//  shader.hpp
//  VI-RT-LPS
//
//  Created by Luis Paulo Santos on 14/03/2023.
//

#ifndef shader_hpp
#define shader_hpp

#include "../Scene/scene.hpp"
#include "../utils/RGB.hpp"

class Shader {
protected:
    Scene *scene;
public:
    Shader (Scene *_scene): scene(_scene) {}
    ~Shader () {}
    virtual RGB shade (bool intersected, Intersection isect, int depth) {return RGB();}
};

#endif /* shader_hpp */
