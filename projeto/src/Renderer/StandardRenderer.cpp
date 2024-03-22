//
//  StandardRenderer.cpp
//  VI-RT-LPS
//
//  Created by Luis Paulo Santos on 14/03/2023.
//

#include "StandardRenderer.hpp"

void StandardRenderer::Render () {
    int W=0,H=0;  // resolution não percebo se e suposto tar a 0 ou meter aqui o valor que ta na main

    // get resolution from the camera
    cam->getResolution(&W, &H);
    std::cout << "Resolution: " << W << "x" << H << "\n";
    
    // main rendering loop: get primary rays from the camera until done
    for (int y=0 ; y< H ; y++) {  // loop over rows
        for (int x=0 ; x< W ; x++) { // loop over columns

            Ray primary;
            Intersection isect;
            bool intersected;
            RGB color;
          
            // Generate Ray (camera)
            bool success = cam->GenerateRay(x, y, &primary);
            // print bullshit

            // trace ray (scene)
            if(success) intersected = scene->trace(primary, &isect);
            // shade this intersection (shader) - remember: depth=0
            color = shd->shade(intersected, isect, 0);
            
            // write the result into the image frame buffer (image)
            img->set(x,y,color);
            if(!success)std::cout << "Didnt give a color: " << success << "\n";

            
        } // loop over columns
    }   // loop over rows
}
