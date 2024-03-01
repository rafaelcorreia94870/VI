//
//  renderer.hpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 10/02/2023.
//

#ifndef renderer_hpp
#define renderer_hpp


#ifdef _WIN32
#include "../Camera/camera.hpp"
#include "../Scene/scene.hpp"
#include "../Image/image.hpp"
#include "../Shader/shader.hpp"

#elif __unix__ || __unix || __linux__ || __APPLE__
#include "camera.hpp"
#include "scene.hpp"
#include "image.hpp"
#include "shader.hpp"
#else
#error "Unsupported operating system"
#endif

class Renderer {
protected:
    Camera *cam;
    Scene *scene;
    Image * img;
    Shader *shd;
public:
    Renderer (Camera *cam, Scene * scene, Image * img, Shader *shd): cam(cam), scene(scene), img(img), shd(shd) {}
    virtual void Render () {}
};

#endif /* renderer_hpp */
