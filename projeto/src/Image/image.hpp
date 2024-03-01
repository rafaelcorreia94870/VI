//
//  image.hpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 10/02/2023.
//

#ifndef image_hpp
#define image_hpp

#include <string>

#ifdef _WIN32
#include "../utils/RGB.hpp"

#elif __unix__ || __unix || __linux__ || __APPLE__
#include "RGB.hpp"
#else
#error "Unsupported operating system"
#endif

class Image {
protected:
    RGB *imagePlane;
    int W,H;
public:
    Image(): W(0),H(0),imagePlane(NULL) {}
    Image(const int W, const int H): W(W),H(H) {
        imagePlane = new RGB[W*H];
        memset((void *)imagePlane, 0, W*H*sizeof(RGB));  // set image plane to 0
    }
    ~Image() {
        if (imagePlane!=NULL) delete[] imagePlane;
    }
    bool set (int x, int y, const RGB &rgb) {
        if (x>W || y>H) return false;
        imagePlane[y*W+x] = rgb;
        return true;
    }
    bool add (int x, int y, const RGB &rgb) {
        if (x>W || y>H) return false;
        imagePlane[y*W+x] += rgb;
        return true;
    }
    bool Save (std::string filename) {return true;}
};

#endif /* image_hpp */
