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
    const bool jitter = false;

    // get resolution from the camera
    cam->getResolution(&W, &H);
    // use pragma omp to parallelize the rendering loop
    // main rendering loop: get primary rays from the camera until done
    #pragma omp parallel for schedule(dynamic, 20) num_threads(20)
    for (int y=0 ; y< H ; y++) {  // loop over rows
        for (int x=0 ; x< W ; x++) { // loop over columns

            Ray primary;
            Intersection isect;
            bool intersected;
            RGB color(0., 0., 0.), this_color(0.,0.,0.);
            for (int ss = 0; ss < spp; ss++) {


                // Generate Ray (camera)
                bool success = false;
                if (jitter) {
                    float jitterV[2];
                    jitterV[0] = ((float)rand()) / ((float)RAND_MAX);
                    jitterV[1] = ((float)rand()) / ((float)RAND_MAX);
                    success = cam->GenerateRay(x, y, &primary, jitterV);
                }
                else {
                    success = cam->GenerateRay(x, y, &primary, NULL);
                }


                // trace ray (scene)
                intersected = scene->trace(primary, &isect);

                // shade this intersection (shader) - remember: depth=0
                this_color = shd->shade(intersected, isect, 0);

                color += this_color;


                // write the result into the image frame buffer (image)
            }
            color = color / float(spp);

            bool setColor = img->set(x, y, color);
            
            if (!setColor) {
				std::cerr << "Error setting color at pixel (" << x << ", " << y << ")" << std::endl;
			}


        } // loop over columns

        //FAZ ISTO MAIS BONITO ROBERT
        if (y == 204) std::cout << "20% done";
        if (y == 204*2) std::cout << "40% done";
        if (y == 204 * 3) std::cout << "60% done";
        if (y == 204 * 4) std::cout << "80% done";
        if (y == 204 * 5) std::cout << "100% done";
    }   // loop over rows
}
