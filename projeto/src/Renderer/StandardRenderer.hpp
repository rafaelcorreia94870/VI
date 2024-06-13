//
//  StandardRenderer.hpp
//  VI-RT-LPS
//
//  Created by Luis Paulo Santos on 14/03/2023.
//

#ifndef StandardRenderer_hpp
#define StandardRenderer_hpp

#include "renderer.hpp"

class StandardRenderer : public Renderer
{
private:
    int spp;
    bool jitter;

public:
    StandardRenderer(Camera *cam, Scene *scene, Image *img, Shader *shd, int _spp, bool _jitter) : Renderer(cam, scene, img, shd)
    {
        spp = _spp;
        jitter = _jitter;
    }
    void Render();
};

#endif /* StandardRenderer_hpp */
