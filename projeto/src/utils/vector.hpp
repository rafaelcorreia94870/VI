//
//  vector.hpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#ifndef vector_hpp
#define vector_hpp

#include <cmath>

class Vector {
public:
    float X,Y,Z;
    Vector ():X(0.),Y(0.),Z(0.){}
    Vector (float x, float y, float z):X(x),Y(y),Z(z){}
    ~Vector(){}
    void set (Vector &v) {
        X = v.X;
        Y = v.Y;
        Z = v.Z;
    }
    Vector operator -(const Vector &p) const { return {X-p.X, Y-p.Y, Z-p.Z};}
    Vector operator +(const Vector &p) const { return {X+p.X, Y+p.Y, Z+p.Z};}
    Vector operator *(const float f) const { return {f*X, f*Y, f*Z};}
    Vector operator *(const double f) const { return {(float)(f*X), (float)(f*Y), (float)(f*Z)};}
    Vector operator /(const float f) const { return {X/f, Y/f, Z/f};}
    friend Vector operator*(const float f, const Vector& p) {
        return p*f;
    }
    friend Vector operator*(const double f, const Vector& p) {
        return p*f;
    }
    // note that methods declared within the class are inline by default
    inline float norm () {
        return sqrtf(X*X+Y*Y+Z*Z); //tinha std::sqrtf
    }
    inline void normalize () {
        const float my_norm = norm();
        if (my_norm>0.) {
            X /= my_norm;
            Y /= my_norm;
            Z /= my_norm;
        }
    }
    float dot (const Vector v2) const {
        return X*v2.X + Y*v2.Y + Z*v2.Z;
    }
    // from pbrt book (3rd ed.), sec 2.2.1, pag 65
    Vector cross (Vector v2) const {
        double v1x = X, v1y = Y, v1z = Z;
        double v2x = v2.X, v2y = v2.Y, v2z = v2.Z;
        return Vector((v1y * v2z) - (v1z * v2y),
        (v1z * v2x) - (v1x * v2z),
        (v1x * v2y) - (v1y * v2x));
    }
    // from pbrt book (3rd ed.), sec 2.2.1, pag 63
    Vector Abs(void) {
        return Vector(std::abs(X), std::abs(Y), std::abs(Z));
    }
    // from pbrt book (3rd ed.), sec 2.2.1, pag 66
    int MaxDimension(void) {
        return (X > Y) ? ((X > Z) ? 0 : 2) : ((Y > Z) ? 1 : 2);
    }
    // from pbrt book (3rd ed.), sec 2.2.1, pag 67
    Vector Permute(int x, int y, int z) {
        const float XYZ[3]={X,Y,Z};
        return Vector(XYZ[x], XYZ[y], XYZ[z]);
    }
    // flip a vector such that it points to the same "side" as v (positive cosine)
    // based on pbrt book, sec 2.4, pag 72
    Vector Faceforward(const Vector &v) const {
        Vector vv = *this;
        return (vv.dot(v) < 0.f) ? -1.f * vv : vv;
    }
    // Generate an orthonormal coordinate system around this vector (must be normalized)
    // returns the 2 new axis orthogonal top the vector
    void CoordinateSystem(Vector *v2, Vector *v3) {
        if (abs(X) > abs(Y))
            *v2 = Vector(-Z, 0, X) / sqrtf(X * X + Z * Z);
        else
            *v2 = Vector(0, Z, -Y) / sqrtf(Y * Y + Z * Z);
        *v3 = cross(*v2);
    }

    // returns a new vector, which is this vector rotated to the
    // reference system defined by Rx, Ry, Rz
    Vector Rotate (Vector Rx, Vector Ry, Vector Rz) {
        Vector vec;
        
        vec.X = X * Rx.X + Y * Ry.X + Z * Rz.X;
        vec.Y = X * Rx.Y + Y * Ry.Y + Z * Rz.Y;
        vec.Z = X * Rx.Z + Y * Ry.Z + Z * Rz.Z;
        return vec;
    }

};

class Point {
protected:      // inherited classes can access this
public:
    float X,Y,Z;
    Point ():X(0.),Y(0.),Z(0.){}
    Point (float x, float y, float z):X(x),Y(y),Z(z){}
    ~Point(){}
    Point operator -(const Point &p) const { return {X-p.X, Y-p.Y, Z-p.Z};}
    Point operator +(const Point &p) const { return {X+p.X, Y+p.Y, Z+p.Z};}
    Point operator *(const float f) const { return {f*X, f*Y, f*Z};}
    Point operator *(const double f) const { return {(float)(f*X), (float)(f*Y), (float)(f*Z)};}
    Point operator +(const Vector &v) const { return {X+v.X, Y+v.Y, Z+v.Z};}
    friend Point operator*(const float f, const Point& p) {
        return p*f;
    }
    friend Point operator*(const double f, const Point& p) {
        return p*f;
    }
    inline void set(float x, float y, float z) {
        X=x;Y=y;Z=z;
    }
    // note that methods declared within the class are inline by default
    inline Vector vec2point (Point p2) {
        Vector v(p2.X-X, p2.Y-Y, p2.Z-Z);
        return v;
    }
    Point Permute(int x, int y, int z) {
        const float XYZ[3]={X,Y,Z};
        return Point(XYZ[x], XYZ[y], XYZ[z]);
    }
};

#endif /* vector_hpp */
