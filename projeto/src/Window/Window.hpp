

#include <GLFW/glfw3.h>



#ifndef WINDOW_HPP
#define WINDOW_HPP
class Window {
private:
    GLFWwindow* window;
    int width, height;
    GLuint textureID;

public:
    Window(int width, int height, const char* title);
    ~Window();
    void render(const unsigned char* imageData);
};

#endif // WINDOW_HPP
