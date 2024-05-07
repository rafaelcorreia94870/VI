//
//  ImagePPM.hpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 09/03/2023.
//

#ifndef ImagePPM_hpp
#define ImagePPM_hpp

#include "image.hpp"

class ImagePPM: public Image {
private:
    typedef struct {
        unsigned char val[3];  // r,g,b
    } PPM_pixel;
    PPM_pixel *imageToSave;
    void ToneMap();

public:
    ImagePPM(const int W, const int H):Image(W, H) {
        imageToSave = new PPM_pixel[W*H];
    }
    ~ImagePPM() {
        if (imageToSave != nullptr) delete[] imageToSave;
    }
    bool Save(std::string filename);
    const unsigned char* getData() const;
    Image readPPM(const char* filename);
};

#endif /* ImagePPM_hpp */

