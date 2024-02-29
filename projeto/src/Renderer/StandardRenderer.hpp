//
//  StandardRenderer.hpp
//  VI-RT-LPS
//
//  Created by Luis Paulo Santos on 14/03/2023.
//

#ifndef StandardRenderer_hpp
#define StandardRenderer_hpp

#include "renderer.hpp"

class StandardRenderer: public Renderer {
private:
    int spp;
public:
    StandardRenderer (Camera *cam, Scene * scene, Image * img, Shader *shd, int _spp): Renderer(cam, scene, img, shd) {
        spp = _spp;
    }
    void Render ();
};

#endif /* StandardRenderer_hpp */
