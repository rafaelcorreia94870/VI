//
//  ImagePPM.cpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 09/03/2023.
//

#include "ImagePPM.hpp"
#include <iostream>
#include <fstream>

void ImagePPM::ToneMap()
{
    imageToSave = new PPM_pixel[W * H];

    for (int j = 0; j < H; ++j)
    {
        for (int i = 0; i < W; ++i)
        {
            imageToSave[j * W + i].val[0] = (unsigned char)(std::min(1.f, imagePlane[j * W + i].R) * 255);
            imageToSave[j * W + i].val[1] = (unsigned char)(std::min(1.f, imagePlane[j * W + i].G) * 255);
            imageToSave[j * W + i].val[2] = (unsigned char)(std::min(1.f, imagePlane[j * W + i].B) * 255);
        }
    }
}

bool ImagePPM::Save(std::string filename)
{
    ToneMap();

    if (W == 0 || H == 0)
    {
        fprintf(stderr, "Can't save an empty image\n");
        return false;
    }

    std::ofstream ofs;
    try
    {
        ofs.open(filename, std::ios::binary);
        if (ofs.fail())
            throw("Can't open output file");

        ofs << "P6\n"
            << W << " " << H << "\n255\n";
        for (int i = 0; i < W * H; ++i)
        {
            ofs << static_cast<unsigned char>(imageToSave[i].val[0])
                << static_cast<unsigned char>(imageToSave[i].val[1])
                << static_cast<unsigned char>(imageToSave[i].val[2]);
        }
        ofs.close();
    }
    catch (const char *err)
    {
        fprintf(stderr, "%s\n", err);
        ofs.close();
        return false;
    }

    return true;
}

Image readPPM(const char *filename)
{
    std::ifstream ifs;
    ifs.open(filename, std::ios::binary);
    // need to spec. binary mode for Windows users
    Image src;
    try
    {
        if (ifs.fail())
        {
            throw("Can't open input file");
        }
        std::string header;
        int w, h, b;
        ifs >> header;
        if (strcmp(header.c_str(), "P6") != 0)
            throw("Can't read input file");
        ifs >> w >> h >> b;
        RGB *imgP = new RGB[w * h];

        ifs.ignore(256, '\n'); // skip empty lines in necessary until we get to the binary data
        unsigned char pix[3];  // read each pixel one by one and convert bytes to floats
        for (int i = 0; i < w * h; ++i)
        {
            ifs.read(reinterpret_cast<char *>(pix), 3);
            imgP[i].R = pix[0] / 255.f;
            imgP[i].G = pix[1] / 255.f;
            imgP[i].B = pix[2] / 255.f;
        }
        ifs.close();
        src.set(w, h, *imgP);
    }
    catch (const char *err)
    {
        fprintf(stderr, "%s\n", err);
        ifs.close();
    }

    return src;
}

const unsigned char *ImagePPM::getData() const
{
    // Convert the image data to the appropriate format for OpenGL
    unsigned char *imageData = new unsigned char[W * H * 3]; // Allocate memory for the image data
    for (int i = 0; i < W * H; ++i)
    {
        imageData[i * 3] = static_cast<unsigned char>(imageToSave[i].val[0]);     // Red channel
        imageData[i * 3 + 1] = static_cast<unsigned char>(imageToSave[i].val[1]); // Green channel
        imageData[i * 3 + 2] = static_cast<unsigned char>(imageToSave[i].val[2]); // Blue channel
    }
    return imageData;
}
