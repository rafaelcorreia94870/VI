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
#include <filesystem>
namespace fs = std::filesystem;
#include <time.h>


int main(int argc, const char* argv[]) {
    Scene scene;
    Perspective* cam; // Camera
    ImagePPM* img;    // Image
    Shader* shd;
    bool success;
    clock_t start, end;
    double cpu_time_used;

    // Image resolution
    int W = 1024;
    int H = 1024;
    //Camera options
    Point Eye ={ 280,275, -330 }, At={280,265, 0};
    float fovW = 90.f;


    // Get custom values from the user
    std::cout << "Enter Eye point (X Y Z), or press Enter to use default (" << Eye.X << " " << Eye.Y << " " << Eye.Z << "): ";
    std::string input;
    std::getline(std::cin, input);
    if (!input.empty()) {
        std::istringstream iss(input);
        float x, y, z;
        iss >> Eye.X >> Eye.Y >> Eye.Z;
    }

    std::cout << "Enter At point (X Y Z), or press Enter to use default (" << At.X << " " << At.Y << " " << At.Z << "): ";
    std::getline(std::cin, input);
    if (!input.empty()) {
        std::istringstream iss(input);
        iss >> At.X >> At.Y >> At.Z;
    }

    std::cout << "Enter Resolution (W H fov), or press Enter to use default (" << W << " " << H << " " << fovW <<"): ";
    std::getline(std::cin, input);
    if (!input.empty()) {
        std::istringstream iss(input);
        iss >> W >> H >> fovW;
    }
    
    #ifdef _WIN32
        std::string relativePath = "..\\src\\Scene\\tinyobjloader\\models\\";
    #elif __unix__ || __unix || __linux__ || __APPLE__
        std::string relativePath = "src/Scene/tinyobjloader/models/";
    #endif

    // Prompt the user to enter the filename
    std::cout << "Please enter the name of the OBJ file (default: cornell_box): ";
    std::string userInput;
    std::getline(std::cin, userInput);

    // Set default value if userInput is empty
    if (userInput.empty()) {
        userInput = "cornell_box";
    }

    // Concatenate the relative path and user input to form the complete file path
    #ifdef _WIN32
        std::string fullPath = fs::current_path().string() + "\\" + relativePath + userInput + ".obj";
    #elif __unix__ || __unix || __linux__ || __APPLE__
        std::string fullPath = fs::current_path().string() + "/" + relativePath + userInput + ".obj";
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
    AmbientLight ambient(RGB(0.9f, 0.9f, 0.9f));
    scene.lights.push_back(&ambient);
    scene.numLights++;



    std::cout << "Scene Load: SUCCESS!! :-)\n";
    scene.printSummary();
    std::cout << std::endl;

    img = new ImagePPM(W, H);

    // Camera parameters
    /*
    const Point Eye = { 0,125,-100 }, At = { 0, 125, 0 };
    */
    const Vector Up={0,1,0};
    const float fovH = fovW * (float)H/(float)W;  // in degrees

    const float fovWrad = fovW*0.0174533f, fovHrad = fovH*0.0174533f;    // to radians

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
