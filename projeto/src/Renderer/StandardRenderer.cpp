//
//  StandardRenderer.cpp
//  VI-RT-LPS
//
//  Created by Luis Paulo Santos on 14/03/2023.
//

#include "StandardRenderer.hpp"
#include <omp.h>

void StandardRenderer::Render () {
    int W=0,H=0;  // resolution não percebo se e suposto tar a 0 ou meter aqui o valor que ta na main

    // get resolution from the camera
    cam->getResolution(&W, &H);
    std::cout << "Resolution: " << W << "x" << H << "\n";
    
    // use pragma omp to parallelize the rendering loop
    #pragma omp parallel for schedule(dynamic, 20) num_threads(16)
    // main rendering loop: get primary rays from the camera until done
    for (int y=0 ; y< H ; y++) {  // loop over rows
        #pragma omp parallel for schedule(dynamic, 20) num_threads(16)
        for (int x=0 ; x< W ; x++) { // loop over columns

            Ray primary;
            Intersection isect;
            bool intersected;
            RGB color;
          
            // Generate Ray (camera)
            bool success = cam->GenerateRay(x, y, &primary);
            if(!success) std::cout << "Failed to generate ray: " << x << " " << y << "\n";
            
            // trace ray (scene)
            if(success) intersected = scene->trace(primary, &isect);
            
            // shade this intersection (shader) - remember: depth=0
            color = shd->shade(intersected, isect, 0);
            
            // write the result into the image frame buffer (image)
            bool setColor = img->set(x,y,color);
            if(!setColor)std::cout << "Didnt give a color: " << success << "\n";

            
        } // loop over columns
    }   // loop over rows
}
