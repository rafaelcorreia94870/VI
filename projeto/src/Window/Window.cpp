#include "Window.hpp"
#include <stdexcept>
#include <iostream>

Window::Window(int width, int height, const char *title) : width(width), height(height), paused(false), running(true)
{
    std::cout << "Initializing GLFW" << std::endl;
    // Initialize GLFW
    if (!glfwInit())
    {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    std::cout << "Creating window" << std::endl;
    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    std::cout << "Making context current" << std::endl;
    // Make the window's context current
    glfwMakeContextCurrent(window);

    std::cout << "Setting up OpenGL" << std::endl;
    // Set up OpenGL viewport and projection
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Set the key callback
    glfwSetKeyCallback(window, keyCallback);
    glfwSetWindowUserPointer(window, this);

    std::cout << "Generating texture" << std::endl;
    // Generate a texture ID
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    // Check for OpenGL errors
    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
    {
        std::cerr << "OpenGL error during initialization: " << err << std::endl;
        throw std::runtime_error("OpenGL error during initialization");
    }
}

Window::~Window()
{
    // Cleanup OpenGL resources
    glDeleteTextures(1, &textureID);
    // Terminate GLFW
    glfwTerminate();
}

void Window::update(const unsigned char *imageData)
{
    if (!imageData)
    {
        std::cerr << "Error: imageData is null" << std::endl;
        return;
    }

    // Bind the texture
    glBindTexture(GL_TEXTURE_2D, textureID);
    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
    {
        std::cerr << "OpenGL error before glTexSubImage2D: " << err << std::endl;
    }

    // Update the texture with new image data
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, imageData);
    err = glGetError();
    if (err != GL_NO_ERROR)
    {
        std::cerr << "OpenGL error during glTexSubImage2D: " << err << std::endl;
    }

    // Enable 2D textures
    glEnable(GL_TEXTURE_2D);

    // Render the updated texture
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 1);
    glVertex2f(0, 0);
    glTexCoord2f(1, 1);
    glVertex2f(width, 0);
    glTexCoord2f(1, 0);
    glVertex2f(width, height);
    glTexCoord2f(0, 0);
    glVertex2f(0, height);
    glEnd();

    glfwSwapBuffers(window);
    glfwPollEvents();
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture

    // Check for OpenGL errors after rendering
    err = glGetError();
    if (err != GL_NO_ERROR)
    {
        std::cerr << "OpenGL error during rendering: " << err << std::endl;
    }
}

void Window::pollEvents() const
{
    // Poll for and process events
    glfwPollEvents();
}

bool Window::shouldClose() const
{
    if (!window)
    {
        std::cerr << "Error: window is null" << std::endl;
        return true;
    }
    return glfwWindowShouldClose(window);
}

void Window::render(const unsigned char *imageData)
{
    // Update the texture with image data
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, imageData);
    glEnable(GL_TEXTURE_2D);

    // Render the updated texture
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 1);
        glVertex2f(0, 0);
        glTexCoord2f(1, 1);
        glVertex2f(width, 0);
        glTexCoord2f(1, 0);
        glVertex2f(width, height);
        glTexCoord2f(0, 0);
        glVertex2f(0, height);
        glEnd();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture
}

void Window::waitUntilClose()
{
    while (!glfwWindowShouldClose(window))
    {
        pollEvents();
    }
}

bool Window::isPaused() const
{
    return paused;
}

bool Window::isRunning() const
{
    return running;
}

void Window::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        Window *win = static_cast<Window *>(glfwGetWindowUserPointer(window));
        if (key == GLFW_KEY_P)
        {
            win->paused = !win->paused; // Toggle pause
            if (win->paused)
            {
                std::cout << "Paused" << std::endl;
            }
            else
            {
                std::cout << "Resumed" << std::endl;
            }
        }
        if (key == GLFW_KEY_Q)
        {
            win->running = false; // Stop rendering
            std::cout << "Closing window" << std::endl;
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    }
}
