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
    #include "utils/customValues.hpp"
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
    #include "customValues.hpp"
#else
    #error "Unsupported operating system"
#endif

#include <filesystem>
namespace fs = std::filesystem;
#include <time.h>

//nao gosto de apontadores, mas e o que temos pra dar
std::vector<AreaLight*> squareLight(Point p, float size) {
    Point v1 = Point(p.X - size, p.Y, p.Z - size);
    Point v2 = Point(p.X + size, p.Y, p.Z - size);
    Point v3 = Point(p.X + size, p.Y, p.Z + size);
    Point v4 = Point(p.X - size, p.Y, p.Z + size);
    Vector n = Vector(0, 1, 0);

    std::vector<AreaLight*> lights; 

    AreaLight* l1 = new AreaLight(RGB(0.65, 0.65, 0.65), v1, v2, v3, n);
    AreaLight* l2 = new AreaLight(RGB(0.65, 0.65, 0.65), v1, v3, v4, n);

    lights.push_back(l1);
    lights.push_back(l2);

    return lights;
}



int main(int argc, const char* argv[]) {
    Scene scene;        // Scene
    Perspective* cam;   // Camera
    ImagePPM* img;      // Image
    Shader* shd;        // Shader
    bool success;   
    clock_t start, end;
    double cpu_time_used;
    
    // File name to load scene
    std::string filename;
    // Image resolution 
    int W, H;
    // Camera setup 
    Point Eye, At;
    float fovW;

    // Options for the user to choose from
    if (argc > 1) {
        // if flag -d is set then use default values
        if (std::string(argv[1]) == "-d") {
            defaultCornellBox(&Eye, &At, &fovW, &W, &H, &filename);
            std::cout << "Using default values for " << filename << std::endl;
        }
        // if flag -t is set then use default values for triangle
        else if (std::string(argv[1]) == "-t"){
            defaultTriangle(&Eye, &At, &fovW, &W, &H, &filename);
            std::cout << "Using default values for " << filename << std::endl;
        }
        // if flag -h is set then print help
        else if (std::string(argv[1]) == "-h"){
            std::cout << "Usage: " << argv[0] << " [-d]" << std::endl;
            std::cout << "Options:" << std::endl;
            std::cout << "  -d Use default values for cornell box" << std::endl;
            std::cout << "  -t Use default values for triangle" << std::endl;
            return 1;
        }
        else {
            std::cout << "Usage: " << argv[0] << " [-d]" << std::endl;
            std::cout << "Options:" << std::endl;
            std::cout << "  -d Use default values for cornell box" << std::endl;
            std::cout << "  -t Use default values for triangle" << std::endl;
            return 1;
        }
    }
    // If no options are set, get custom values from the user
    else {
        getCustomValues(&Eye, &At, &fovW, &W, &H, &filename);
    }

    // Concatenate the relative path and user input to form the complete file path
    #ifdef _WIN32
        std::string relativePath = "..\\src\\Scene\\tinyobjloader\\models\\";
        std::string fullPath = fs::current_path().string() + "\\" + relativePath + filename + ".obj";
    #elif __unix__ || __unix || __linux__ || __APPLE__
        std::string relativePath = "src/Scene/tinyobjloader/models/";
        std::string fullPath = fs::current_path().string() + "/" + relativePath + filename + ".obj";
    #endif

    // Convert the file path to a filesystem path
    fs::path path(fullPath);
        // Check if the file exists
        if (!fs::exists(path)) {
            std::cerr << "Error: File does not exist." << std::endl;
            return 1;
        }

    // Load the scene using the provided file path
    std::string pathStr = path.string();
    success = scene.Load(pathStr);
        if (!success) {
            std::cout << "ERROR!! :o\n";
            return 1;
        }

    // add an ambient light to the scene
    
    AmbientLight ambient(RGB(0.05f, 0.05f, 0.05f));

    scene.lights.push_back(&ambient);
    scene.numLights++;

    /*
    PointLight pl1(RGB(0.65, 0.65, 0.65), Point(288, 508, 282));
    scene.lights.push_back(&pl1);
    scene.numLights++;

    //make a circle of 10 lights around the pl1 light
    for (int i = 0; i < 10; i++) {
		float angle = i * 36.0f;
		float x = 288 + 25 * cos(angle);
		float z = 282 + 25 * sin(angle);
		PointLight* pl = new PointLight(RGB(0.05, 0.05, 0.05), Point(x, 508, z));
		scene.lights.push_back(pl);
		scene.numLights++;
	}

    for (int i = 0; i < 10; i++) {
        float angle = i * 36.0f;
        float x = 288 + 50 * cos(angle);
        float z = 282 + 50 * sin(angle);
        PointLight* pl = new PointLight(RGB(0.10, 0.10, 0.10), Point(x, 508, z));
        scene.lights.push_back(pl);
        scene.numLights++;
    }
    */
    //(RGB _power, Point _v1, Point _v2, Point _v3, Vector _n

    /*
    std::vector<AreaLight*> light_square = squareLight(Point(288, 508, 282), 4);

    for (const auto& light : light_square) {
        scene.lights.push_back(light);
		scene.numLights++;
    }
    */
    float size = 4;
    Point p = Point(288, 508, 282);
    Point v1 = Point(p.X - size, p.Y, p.Z - size);
    Point v2 = Point(p.X + size, p.Y, p.Z - size);
    Point v3 = Point(p.X + size, p.Y, p.Z + size);
    Point v4 = Point(p.X - size, p.Y, p.Z + size);
    Vector n = Vector(0, 1, 0);

    AreaLight l1 = AreaLight(RGB(0.65, 0.65, 0.65), v1, v2, v3, n);
    scene.lights.push_back(&l1);
    scene.numLights++;
    AreaLight l2 = AreaLight(RGB(0.65, 0.65, 0.65), v1, v3, v4, n);
    scene.lights.push_back(&l2);
    scene.numLights++;

    // scene details
    std::cout << "Scene Load: SUCCESS!! :-)\n";
    scene.printSummary();
    std::cout << std::endl;

    // create the image
    img = new ImagePPM(W, H);

    // create the camera
    const Vector Up={0,1,0};                                             // up vector
    const float fovH = fovW * (float)H/(float)W;                         // in degrees
    const float fovWrad = fovW*0.0174533f, fovHrad = fovH*0.0174533f;    // to radians
    cam = new Perspective(Eye, At, Up, W, H, fovWrad, fovHrad);          // camera
    
    // create the ambient shader
    RGB background(0.2f, 0.2f, 0.2f);
    //shd = new WhittedShader(&scene, background);
    shd = new DistributedShader(&scene, background);


    // declare the renderer
    int spp=1;     // samples per pixel
    StandardRenderer myRender (cam, &scene, img, shd, spp);
    // render
    start = clock();
    myRender.Render();
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    // save the image to a file
    img->Save(filename + "_rendered.ppm");
    
    // print rendering time
    fprintf (stdout, "Rendering time = %.3lf secs\n\n", cpu_time_used);
    std::cout << "That's all, folks!" << std::endl;
    return 0;
}
