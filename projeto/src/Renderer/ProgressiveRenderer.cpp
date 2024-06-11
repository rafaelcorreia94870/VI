#include "ProgressiveRenderer.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>

void ProgressiveRenderer::render()
{
    std::cout << "Progressive rendering started." << std::endl;
    cam->getResolution(&width, &height);

    // Initialize the accumulation buffer with zeros
    accumulationBuffer.resize(width * height * 3, 0.0f);

    // Loop for progressive rendering using spp samples per iteration
    int ss = 0;
    while (window->isRunning())
    {
        if (!window->isPaused())
        {
            ss++;
            std::cout << "Rendering sample " << ss << std::endl;
            renderFrame(width, height);

            // Save intermediate image
            /* std::ostringstream filename;
            filename << "progressive_output_" << std::setw(4) << std::setfill('0') << ss << ".ppm";
            img->Save(filename.str());
            std::cout << "Frame " << ss << " completed." << std::endl; */

            // Update the window with the intermediate image
            const unsigned char *imageData = img->getCharData();
            window->update(imageData);
        }

        // Poll events regardless of pause state
        window->pollEvents();
    }

    // Normalize the accumulated colors by the number of samples per pixel
    img->normalize(ss);

    // Keep the window open until the user closes it
    window->waitUntilClose();
}

void ProgressiveRenderer::renderFrame(int width, int height)
{
    const bool jitter = true;

    // Perform path tracing for one frame
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            // Create a ray for the current pixel
            Ray primary;
            bool success = false;
            if (jitter)
            {
                float jitterV[2];
                jitterV[0] = ((float)rand()) / ((float)RAND_MAX);
                jitterV[1] = ((float)rand()) / ((float)RAND_MAX);
                success = cam->GenerateRay(x, y, &primary, jitterV);
            }
            else
            {
                success = cam->GenerateRay(x, y, &primary);
            }

            if (!success)
            {
                std::cerr << "Failed to generate primary ray at (" << x << ", " << y << ")" << std::endl;
                continue;
            }

            // Trace the ray and get the intersection
            Intersection isect;
            bool intersected = scene->trace(primary, &isect);

            // Shade the intersection point
            RGB color = shd->shade(intersected, isect, 0);

            // Accumulate the color
            accumulationBuffer[(y * width + x) * 3 + 0] += color.R;
            accumulationBuffer[(y * width + x) * 3 + 1] += color.G;
            accumulationBuffer[(y * width + x) * 3 + 2] += color.B;

            // Set the accumulated color to the image for display purposes
            RGB accumulatedColor(
                accumulationBuffer[(y * width + x) * 3 + 0] / frameCount,
                accumulationBuffer[(y * width + x) * 3 + 1] / frameCount,
                accumulationBuffer[(y * width + x) * 3 + 2] / frameCount);

            if (!img->set(x, y, accumulatedColor))
            {
                std::cerr << "Error setting color at pixel (" << x << ", " << y << ")" << std::endl;
            }
        }
    }

    frameCount++;
}
