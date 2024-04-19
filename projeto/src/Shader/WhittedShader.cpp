//
//  AmbientShader.cpp
//  VI-RT-LPS
//
//  Created by Luis Paulo Santos on 14/03/2023.
//

#include "WhittedShader.hpp"

RGB WhittedShader::directLighting(Intersection isect, Phong* f) {
    RGB color(0., 0., 0.);

    // Loop over scene's light sources
    for (auto l = scene->lights.begin(); l != scene->lights.end(); l++) {

        if ((*l)->type == AMBIENT_LIGHT) {  // is it an ambient light ?
            if (!f->Ka.isZero()) {
                RGB Ka = f->Ka;
                color += Ka * (*l)->L();
            }
            continue;
        }
        if ((*l)->type == POINT_LIGHT) {  // is it a point light ?
            if (!f->Kd.isZero()) {
                Point lpoint;
                // get the position and radiance of the light source
                RGB L = (*l)->Sample_L(NULL, &lpoint);

                // compute the direction from the intersection to the light
                Vector Ldir = isect.p.vec2point(lpoint);
                const float Ldistance = Ldir.norm();
                Ldir.normalize(); // now normalize Ldir

                // compute the cosine (Ldir , shading normal) 
                float cosL = Ldir.dot(isect.sn);
                if (cosL > 0.) { // the light is NOT behind the primitive
                    // generate the shadow ray
                    Ray shadow(isect.p, Ldir);
                    // adjust origin EPSILON along the normal: avoid self occlusion 
                    shadow.adjustOrigin(isect.gn);
                    if (scene->visibility(shadow, Ldistance - EPSILON))// light source not occluded
                        color += f->Kd * L * cosL;
                } // end cosL > 0.


            }
            continue;

        }
    }
    return color;
}

RGB WhittedShader::specularReflection(Intersection isect, Phong* f, int depth) {
    // generate the specular ray
    float cos = isect.gn.dot(isect.wo);
    Vector Rdir = 2.f * cos * isect.gn - isect.wo;
    Ray specular(isect.p, Rdir);
    specular.adjustOrigin(isect.gn);

    Intersection s_isect;
    // trace ray
    bool intersected = scene->trace(specular, &s_isect);
    // shade this intersection
    RGB color = shade(intersected, s_isect, depth + 1);
    return color;


}

RGB WhittedShader::shade(bool intersected, Intersection isect, int depth) {
    RGB color(0., 0., 0.);

    if (!intersected) { return (background); }
    // get the BRDF
    Phong* f = (Phong*)isect.f;

    // if there is a specular component sample it
    if (!f->Ks.isZero() && depth < 3) {
        color += specularReflection(isect, f, depth + 1);
    }

    color += directLighting(isect, f);

    return color;


};
