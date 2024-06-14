#include "ProgressiveRenderer.hpp"
#include "../Image/ImagePPM.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>

void ProgressiveRenderer::render()
{
    std::cout << "Progressive rendering started." << std::endl;
    cam->getResolution(&width, &height);

    accumulationBuffer.resize(width * height * 3, 0.0f);

    int ss = 0;
    bool done = false;

    while (window->isRunning() && !done)
    {
        if (!window->isPaused())
        {
            ss++;
            std::cout << "Rendering sample " << ss << std::endl;
            renderFrame(width, height);

            const unsigned char *imageData = img->getCharData();
            window->update(imageData);

            if (ss % 256 == 0 || ss == 1)
            {
                // Save intermediate image for debugging
                std::ostringstream filename;
                filename << "intermediate_imgs/progressive_output_" << std::setw(4) << std::setfill('0') << ss << ".ppm";
                std::cout << "Saving intermediate image to " << filename.str() << std::endl;

                ImagePPM *imgPPM = dynamic_cast<ImagePPM *>(img);
                if (imgPPM && !imgPPM->Save(filename.str()))
                {
                    std::cerr << "Failed to save image to " << filename.str() << std::endl;
                }
            }
        }

        window->pollEvents();

        if (spp > 0)
        {
            done = ss >= spp;
        }
    }

    std::cout << "Progressive rendering finished." << std::endl;
    // Normalize and save the final image
    img->Save("final_output.ppm");
    window->waitUntilClose();
}

void ProgressiveRenderer::renderFrame(int width, int height)
{
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
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

            Intersection isect;
            bool intersected = scene->trace(primary, &isect);

            RGB color = shd->shade(intersected, isect, 0);

            accumulationBuffer[(y * width + x) * 3 + 0] += color.R;
            accumulationBuffer[(y * width + x) * 3 + 1] += color.G;
            accumulationBuffer[(y * width + x) * 3 + 2] += color.B;

            RGB accumulatedColor(
                accumulationBuffer[(y * width + x) * 3 + 0] / (frameCount + 1),
                accumulationBuffer[(y * width + x) * 3 + 1] / (frameCount + 1),
                accumulationBuffer[(y * width + x) * 3 + 2] / (frameCount + 1));

            if (!img->set(x, y, accumulatedColor))
            {
                std::cerr << "Error setting color at pixel (" << x << ", " << y << ")" << std::endl;
            }
        }
    }

    frameCount++;
}
