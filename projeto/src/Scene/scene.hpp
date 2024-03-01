//
//  Scene.hpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#ifndef Scene_hpp
#define Scene_hpp

#include <iostream>
#include <string>
#include <vector>


#ifdef _WIN32
#include "../Primitive/primitive.hpp"
#include "../Light/light.hpp"
#include "../Rays/ray.hpp"
#include "../Rays/intersection.hpp"
#include "../Primitive/BRDF/BRDF.hpp"

#elif __unix__ || __unix || __linux__ || __APPLE__
#include "primitive.hpp"
#include "light.hpp"
#include "ray.hpp"
#include "intersection.hpp"
#include "BRDF.hpp"
#else
#error "Unsupported operating system"
#endif

class Scene {
    std::vector <Primitive *> prims;
    std::vector <BRDF *> BRDFs;
public:
    std::vector <Light *> lights;
    int numPrimitives, numLights, numBRDFs;

    Scene (): numPrimitives(0), numLights(0), numBRDFs(0) {}
    bool Load (const std::string &fname);
    bool SetLights (void) { return true; };
    bool trace (Ray r, Intersection *isect);
    bool visibility (Ray s, const float maxL);
    void printSummary(void) {
        std::cout << "#primitives = " << numPrimitives << " ; ";
        std::cout << "#lights = " << numLights << " ; ";
        std::cout << "#materials = " << numBRDFs << " ;" << std::endl;
    }
};

#endif /* Scene_hpp */
