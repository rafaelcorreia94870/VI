//
//  AmbientShader.cpp
//  VI-RT-LPS
//
//  Created by Luis Paulo Santos on 14/03/2023.
//

#include "AmbientShader.hpp"
#include "Phong.hpp"

RGB AmbientShader::shade(bool intersected, Intersection isect, int depth) {
    RGB color(0.,0.,0.);

    // if no intersection, return background
    if (!intersected) {
        return (background);
    }
    
    if (isect.isLight) { // intersection with a light source
    //print isect info Le
        printf("Le: (%f, %f, %f)\n\n", isect.Le.R, isect.Le.G, isect.Le.B);
        return isect.Le;
    }
    
    // verify whether the intersected object has an ambient component
    Phong *f = (Phong *)isect.f;
    if (f->Ka.isZero()) return color;
    RGB Ka = f->Ka;
    // ambient shade
    // Loop over scene's light sources and process Ambient Lights
    //print lights 
    for (auto light_itr = scene->lights.begin() ; light_itr != scene->lights.end() ; light_itr++) {
        
        if ((*light_itr)->type == AMBIENT_LIGHT) {  // is it an ambient light ?
            color += Ka * (*light_itr)->L();
            continue;
        }
    }
        
    return color;
};