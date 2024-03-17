//
//  main.cpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#include <iostream>
#ifdef _WIN32
#include "Scene/scene.hpp"
#include "Camera/perspective.hpp"
#include "Renderer/StandardRenderer.hpp"
#include "Image/ImagePPM.hpp"
#include "Shader/AmbientShader.hpp"
#include "Shader/WhittedShader.hpp"
#include "Shader/DistributedShader.hpp"
#include "Shader/PathTracerShader.hpp"
#include "Light/AmbientLight.hpp"
#include "Light/PointLight.hpp"
#include "Light/AreaLight.hpp"
#include <filesystem>
namespace fs = std::filesystem;
#elif __unix__ || __unix || __linux__ || __APPLE__
#include "scene.hpp"
#include "perspective.hpp"
#include "StandardRenderer.hpp"
#include "ImagePPM.hpp"
#include "AmbientShader.hpp"
#include "WhittedShader.hpp"
#include "DistributedShader.hpp"
#include "PathTracerShader.hpp"
#include "AmbientLight.hpp"
#include "PointLight.hpp"
#include "AreaLight.hpp"
#else
#error "Unsupported operating system"
#endif

#include <time.h>


int main(int argc, const char * argv[]) {
    Scene scene;
    Perspective *cam; // Camera
    ImagePPM *img;    // Image
    Shader *shd;
    bool success;
    clock_t start, end;
    double cpu_time_used;
    #ifdef _WIN32
    fs::path currentPath = fs::current_path();
    fs::path path = currentPath /".." / "src" / "Scene" / "tinyobjloader" / "models" / "cornell_box.obj";
    std::string pathStr = path.string();
    success = scene.Load(pathStr);
    #elif __unix__ || __unix || __linux__
    success = scene.Load("/home/robert/aulas/4ano/2sem/VI/TP/VI/projeto/src/Scene/tinyobjloader/models/cornell_box.obj");
    #elif __APPLE__
    sucess = scene.load("/Users/psantos/VI-RT/VI-RT/VI-RT/Scene/tinyobjloader/models/cornell_box.obj");
    #endif
    if (!success) {
        std::cout << "ERROR!! :o\n";
        return 1;
    }    
    // add an ambient light to the scene
    AmbientLight ambient(RGB(0.9f,0.9f,0.9f));
    scene.lights.push_back(&ambient);
    scene.numLights++;

    std::cout << "Scene Load: SUCCESS!! :-)\n";
    scene.printSummary();
    std::cout << std::endl;

    // Image resolution
    const int W = 24;
    const int H= 24;
    
    img = new ImagePPM(W,H);
    
    // Camera parameters
    const Point Eye ={ 280,275, -330 }, At={280,265, 0};
    const Vector Up={0,1,0};
    const float fovW = 90.f;
    const float fovH = fovW * (float)H/(float)W;  // in degrees
    const float fovWrad = fovW*3.14f/180.f, fovHrad = fovH*3.14f/180.f;    // to radians
    cam = new Perspective(Eye, At, Up, W, H, fovWrad, fovHrad);
    //cam->PrintInfo();
    
    // create the shader
    RGB background(0.05f, 0.05f, 0.55f);
    shd = new AmbientShader(&scene, background);
    // declare the renderer
    int spp=1;     // samples per pixel
    StandardRenderer myRender (cam, &scene, img, shd, spp);
    // render
    start = clock();
    myRender.Render();
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    // save the image
    img->Save("MyImage.ppm");
    
    fprintf (stdout, "Rendering time = %.3lf secs\n\n", cpu_time_used);
    
    std::cout << "That's all, folks!" << std::endl;
    return 0;
}
