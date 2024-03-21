//
//  AmbientShader.cpp
//  VI-RT-LPS
//
//  Created by Luis Paulo Santos on 14/03/2023.
//

#include "AmbientShader.hpp"

RGB AmbientShader::shade(bool intersected, Intersection isect, int depth) {
    RGB color(0.,0.,0.);
        
    std::cout << isect.pix_x << " " << isect.pix_y << "\n";

    // for pixel 63,63 print isect.Le
    if (isect.pix_x == 63 && isect.pix_y == 63){
        std::cout << "Le: " << isect.Le.R << " " << isect.Le.G << " " << isect.Le.B << "\n";
    }
    // if no intersection, return background
    if (!intersected) {
        return (background);
    }
    if (isect.isLight) { // intersection with a light source
        return isect.Le;
    }
    
    
    // verify whether the intersected object has an ambient component
    Phong *f = (Phong *)isect.f;
    if (f->Ka.isZero()) return color;
    RGB Ka = f->Ka;
    
    // ambient shade
    // Loop over scene's light sources and process Ambient Lights
    for (auto light_itr = scene->lights.begin() ; light_itr != scene->lights.end() ; light_itr++) {
        
        if ((*light_itr)->type == AMBIENT_LIGHT) {  // is it an ambient light ?
            color += Ka * (*light_itr)->L();
            continue;
        }
    }
        
    return color;
};
