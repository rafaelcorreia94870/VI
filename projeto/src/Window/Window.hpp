#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <GLFW/glfw3.h>

class Window
{
private:
    GLFWwindow *window;
    int width, height;
    GLuint textureID;
    bool paused;
    bool running;

    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

public:
    Window(int width, int height, const char *title);
    ~Window();
    void render(const unsigned char *imageData);
    void update(const unsigned char *imageData);
    void pollEvents() const;
    bool shouldClose() const;
    void waitUntilClose();
    bool isPaused() const;
    bool isRunning() const;
};

#endif // WINDOW_HPP
