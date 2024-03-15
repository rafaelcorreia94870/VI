//
//  StandardRenderer.cpp
//  VI-RT-LPS
//
//  Created by Luis Paulo Santos on 14/03/2023.
//

#include "StandardRenderer.hpp"

void StandardRenderer::Render () {
    int W=0,H=0;  // resolution não percebo se e suposto tar a 0 ou meter aqui o valor que ta na main
    int x,y;

    // get resolution from the camera
    cam->getResolution(&W, &H);
    
    // main rendering loop: get primary rays from the camera until done
    for (y=0 ; y< H ; y++) {  // loop over rows
        for (x=0 ; x< W ; x++) { // loop over columns
            Ray primary;
            Intersection isect;
            bool intersected;
            RGB color;
          
            // Generate Ray (camera)
            bool success = cam->GenerateRay(x, y, &primary);
            //if(!success)std::cout << "Cam generated a Ray: " << success << "\n";
            
            // trace ray (scene)
            intersected = scene->trace(primary, &isect);
            //if (intersected)std::cout << "Ray intersected scene : " << intersected << "\n";
            
            // shade this intersection (shader) - remember: depth=0
            color = shd->shade(intersected, isect, 0);
            
            // write the result into the image frame buffer (image)
            img->set(x,y,color);
            
        } // loop over columns
    }   // loop over rows
}
