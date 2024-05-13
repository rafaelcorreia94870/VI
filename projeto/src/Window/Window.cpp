#include "Window.hpp"
#include <stdexcept>
#include <iostream>



Window::Window(int width, int height, const char* title) : width(width), height(height) {
    // Initialize GLFW
    if (!glfwInit()) {
        // Handle initialization failure
        throw std::runtime_error("Failed to initialize GLFW");
    }

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window) {
        // Handle window creation failure
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Set up OpenGL viewport and projection
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Bind default framebuffer
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

Window::~Window() {
    // Cleanup OpenGL resources
    glDeleteTextures(1, &textureID);

    // Terminate GLFW
    glfwTerminate();
}

void Window::render(const unsigned char* imageData) {
    
    // Update the texture with image data
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);

    // Enable 2D textures
    glEnable(GL_TEXTURE_2D);

    // Rendering loop
    while (!glfwWindowShouldClose(window)) {
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw the textured quad covering the entire window
        glBegin(GL_QUADS);
        glTexCoord2f(0, 1); glVertex2f(0, 0);          // Bottom-left
        glTexCoord2f(1, 1); glVertex2f(width, 0);     // Bottom-right
        glTexCoord2f(1, 0); glVertex2f(width, height); // Top-right
        glTexCoord2f(0, 0); glVertex2f(0, height);    // Top-left
        glEnd();

        // Swap buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture

}
