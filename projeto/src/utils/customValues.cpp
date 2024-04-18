#ifdef _WIN32
    #include "../utils/customValues.hpp"    
#elif __unix__ || __unix || __linux__ || __APPLE__
    #include "customValues.hpp"
#else
    #error "Unsupported operating system"
#endif
// CHANGE DEFAULT VALUES HERE
// global default values for eyeX, eyeY, eyeZ
#define eyeX 280
#define eyeY 275
#define eyeZ -330
// global default values for atX, atY, atZ
#define atX 280
#define atY 265
#define atZ 0
// global default values for W, H, fovW
#define dW 1024
#define dH 1024
#define dfovW 90.0f
// global default values for filename
#define dfilename "cornell_box_VI"



void getCustomEyePoint(Point *Eye) {
    // Get custom values from the user
    std::cout << "Enter Eye point (X Y Z), or press Enter to use default (" << eyeX << ", " << eyeY << ", " << eyeZ << "): ";
    std::string input;
    std::getline(std::cin, input);
    if (!input.empty()) {
        std::istringstream iss(input);
        iss >> Eye->X >> Eye->Y >> Eye->Z;
    }
    else {
        Eye->X = eyeX;
        Eye->Y = eyeY;
        Eye->Z = eyeZ;
    }
}

void getCustomAtPoint(Point *At) {
    // Get custom values from the user
    std::cout << "Enter At point (X Y Z), or press Enter to use default (" << atX << ", " << atY << ", " << atZ << "): ";
    std::string input;
    std::getline(std::cin, input);
    if (!input.empty()) {
        std::istringstream iss(input);
        iss >> At->X >> At->Y >> At->Z;
    }
    else {
        At->X = atX;
        At->Y = atY;
        At->Z = atZ;
    }
}

void getCustomFov(float *fovW, int *W, int *H) {
    std::cout << "Enter Resolution (W H fov), or press Enter to use default (" << dW << ", " << dH << ", " << dfovW << "): ";
    std::string input;
    std::getline(std::cin, input);

    if (!input.empty()) {
        std::istringstream iss(input);
        iss >> *W >> *H >> *fovW;
    }
    else {
        *W = dW;
        *H = dH;
        *fovW = dfovW;
    }
}

void getCustomFilename(std::string *userInput) {
    // Prompt the user to enter the filename
    std::cout << "Please enter the name of the OBJ file (default: " << dfilename << "): ";
    std::getline(std::cin, *userInput);
    if (userInput->empty()) {
        *userInput = dfilename;
    }
}

void defaultCornellBox(Point *Eye, Point *At, float *fovW, int *W, int *H, std::string *filename) {
    *W = 1024;
    *H = 1024;
    *Eye ={280, 275, -330}, *At = {280, 265, 0};
    *fovW = 90.f;
    *filename = "cornell_box";
}

void defaultTriangle(Point *Eye, Point *At, float *fovW, int *W, int *H, std::string *filename) {
    *W = 512;
    *H = 512;
    *Eye ={0, 125, -200}, *At={0, 125, 0};
    *fovW = 90.f;
    *filename = "triangle";
}

void getCustomValues(Point *Eye, Point *At, float *fovW, int *W, int *H, std::string *filename) {
    // Get custom values from the user
    getCustomEyePoint(Eye);
    getCustomAtPoint(At);
    getCustomFov(fovW, W, H);
    getCustomFilename(filename);
}