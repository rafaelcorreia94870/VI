#ifndef PROGRESSIVE_RENDERER_HPP
#define PROGRESSIVE_RENDERER_HPP

#include "renderer.hpp"
#include "Window.hpp"
#include <vector>

class ProgressiveRenderer : public Renderer
{
private:
    std::vector<float> accumulationBuffer;
    int spp;
    int width;
    int height;
    int frameCount;
    bool jitter;
    Window *window; // Add this member
public:
    ProgressiveRenderer(Camera *cam, Scene *scene, Image *img, Shader *shd, int _spp, Window *window, bool _jitter)
        : Renderer(cam, scene, img, shd), window(window)
    {
        spp = _spp;
        jitter = _jitter;
        frameCount = 0;
    }

    void render();
    void renderFrame(int width, int height);
};

#endif // PROGRESSIVE_RENDERER_HPP
