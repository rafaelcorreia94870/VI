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
#define dW 256
#define dH 256
#define dfovW 90.0f
// global default values for filename
#define dfilename "cornell_box_VI"

// global default value for spp
#define dspp 16

void getCustomEyePoint(Point *Eye)
{
    // Get custom values from the user
    std::cout << "Enter Eye point (X Y Z), or press Enter to use default (" << eyeX << ", " << eyeY << ", " << eyeZ << "): ";
    std::string input;
    std::getline(std::cin, input);
    if (!input.empty())
    {
        std::istringstream iss(input);
        iss >> Eye->X >> Eye->Y >> Eye->Z;
    }
    else
    {
        Eye->X = eyeX;
        Eye->Y = eyeY;
        Eye->Z = eyeZ;
    }
}

void getCustomAtPoint(Point *At)
{
    // Get custom values from the user
    std::cout << "Enter At point (X Y Z), or press Enter to use default (" << atX << ", " << atY << ", " << atZ << "): ";
    std::string input;
    std::getline(std::cin, input);
    if (!input.empty())
    {
        std::istringstream iss(input);
        iss >> At->X >> At->Y >> At->Z;
    }
    else
    {
        At->X = atX;
        At->Y = atY;
        At->Z = atZ;
    }
}

void getCustomFov(float *fovW, int *W, int *H)
{
    std::cout << "Enter Resolution (W H fov), or press Enter to use default (" << dW << ", " << dH << ", " << dfovW << "): ";
    std::string input;
    std::getline(std::cin, input);

    if (!input.empty())
    {
        std::istringstream iss(input);
        iss >> *W >> *H >> *fovW;
    }
    else
    {
        *W = dW;
        *H = dH;
        *fovW = dfovW;
    }
}

void getCustomFilename(std::string *userInput)
{
    // Prompt the user to enter the filename
    std::cout << "Please enter the name of the OBJ file (default: " << dfilename << "): ";
    std::getline(std::cin, *userInput);
    if (userInput->empty())
    {
        *userInput = dfilename;
    }
}

void defaultCornellBox(Point *Eye, Point *At, float *fovW, int *W, int *H, int *spp, std::string *filename)
{
    *W = dW;
    *H = dH;
    *Eye = {eyeX, eyeY, eyeZ}, *At = {atX, atY, atZ};
    *fovW = dfovW;
    *filename = dfilename;
    *spp = dspp;
}

void defaultTriangle(Point *Eye, Point *At, float *fovW, int *W, int *H, int *spp, std::string *filename)
{
    *W = 512;
    *H = 512;
    *Eye = {0, 125, -200}, *At = {0, 125, 0};
    *fovW = 90.f;
    *filename = "triangle";
    *spp = 4;
}

void getCustomValues(Point *Eye, Point *At, float *fovW, int *W, int *H, int *spp, std::string *filename)
{
    // Get custom values from the user
    getCustomEyePoint(Eye);
    getCustomAtPoint(At);
    getCustomFov(fovW, W, H);
    getCustomFilename(filename);
    getCustomSPP(spp);
}

void getCustomSPP(int *spp)
{
    // Get custom values from the user
    std::cout << "Enter the number of samples per pixel (default:" << dspp << "): ";
    std::string input;
    std::getline(std::cin, input);
    if (!input.empty())
    {
        std::istringstream iss(input);
        iss >> *spp;
    }
    else
    {
        *spp = dspp;
    }
}

void getShaderType(int *shaderType)
{
    std::cout << "Enter the shader type (default: PathTracing)\n  W: Whitted \n  D: Distributed\n  P: PathTracing\n  > ";
    std::string input;
    std::getline(std::cin, input);
    if (!input.empty())
    {
        if (input == "W")
        {
            *shaderType = 0;
        }
        else if (input == "D")
        {
            *shaderType = 1;
        }
        else if (input == "P")
        {
            *shaderType = 2;
        }
        else
        {
            std::cout << "Invalid input. Using default shader type (Path Tracing)" << std::endl;
            *shaderType = 2;
        }
    }
    else
    {
        *shaderType = 2;
    }
}
void getRenderType(int *renderType)
{
    std::cout << "Enter the render type (default: Progressive)\n  S: Standard \n  P: Progressive\n  > ";
    std::string input;
    std::getline(std::cin, input);
    if (!input.empty())
    {
        if (input == "S")
        {
            *renderType = 0;
        }
        else if (input == "P")
        {
            *renderType = 1;
        }
        else
        {
            std::cout << "Invalid input. Using default render type (Progressive)" << std::endl;
            *renderType = 1;
        }
    }
    else
    {
        *renderType = 1;
    }
}

void getNumLights(int *numLights)
{
    std::cout << "Enter the number of lights (1: 1 light, 5: 5 lights): ";
    std::string input;
    std::getline(std::cin, input);
    if (!input.empty())
    {
        if (input == "1")
        {
            *numLights = 1;
        }
        else if (input == "5")
        {
            *numLights = 5;
        }
        else
        {
            std::cout << "Invalid input. Using default number of lights (1 light)" << std::endl;
            *numLights = 1;
        }
    }
    else
    {
        *numLights = 1;
    }
}

void getJitter(bool *jitter)
{
    std::cout << "Enable jitter? (y/n): ";
    std::string input;
    std::getline(std::cin, input);
    if (!input.empty())
    {
        if (input == "y")
        {
            *jitter = true;
        }
        else if (input == "n")
        {
            *jitter = false;
        }
        else
        {
            std::cout << "Invalid input. Using default value: jitter" << std::endl;
            *jitter = true;
        }
    }
    else
    {
        *jitter = true;
    }
}

void getRussianRoulette(bool *russianRoulette)
{
    std::cout << "Enable Russian Roulette? (y/n): ";
    std::string input;
    std::getline(std::cin, input);
    if (!input.empty())
    {
        if (input == "y")
        {
            *russianRoulette = true;
        }
        else if (input == "n")
        {
            *russianRoulette = false;
        }
        else
        {
            std::cout << "Invalid input. Using default value: Russian Roulette" << std::endl;
            *russianRoulette = true;
        }
    }
    else
    {
        *russianRoulette = true;
    }
}
