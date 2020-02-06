// High level design note
// Our matrix should match the behavior of the glm library.
#ifndef MATRIX4F_H
#define MATRIX4F_H

#include <cmath>

// We need to Vector4f header in order to multiply a matrix
// by a vector.
#include "Vector4f.h"

// Matrix 4f represents 4x4 matrices in Math
struct Matrix4f{
public:
    float n[4][4];  // Store each value of the matrix
    Matrix4f() = default;

    // Column major order!
    // Matrix constructor with 9 scalar values.
    Matrix4f( float n00, float n01, float n02, float n03,
              float n10, float n11, float n12, float n13,
              float n20, float n21, float n22, float n23,
              float n30, float n31, float n32, float n33){

        n[0][0] = n00; n[0][1] = n10; n[0][2] = n20; n[0][3] = n30;
        n[1][0] = n01; n[1][1] = n11; n[1][2] = n21; n[1][3] = n31;
        n[2][0] = n02; n[2][1] = n12; n[2][2] = n22; n[2][3] = n32;
        n[3][0] = n03; n[3][1] = n13; n[3][2] = n23; n[3][3] = n33;
    }

    // Matrix constructor from four vectors.
    // Note: 'd' will almost always be 0,0,0,1
    Matrix4f(const Vector4f& a, const Vector4f& b, const Vector4f& c, const Vector4f& d){
        init(a,b,c,d);
    }

    void init(const Vector4f& a, const Vector4f& b, const Vector4f& c, const Vector4f& d){
        n[0][0] = a.x; n[0][1] = b.x; n[0][2] = c.x; n[0][3] = d.x;
        n[1][0] = a.y; n[1][1] = b.y; n[1][2] = c.y; n[1][3] = d.y;
        n[2][0] = a.z; n[2][1] = b.z; n[2][2] = c.z; n[2][3] = d.z;
        n[3][0] = a.w; n[3][1] = b.w; n[3][2] = c.w; n[3][3] = d.w;
    }

    // Makes the matrix an identity matrix
    void identity(){
        init(Vector4f(1,0,0,0),
             Vector4f(0,1,0,0),
             Vector4f(0,0,1,0),
             Vector4f(0,0,0,1));
    }

    // Index operator with two dimensions
    // Example: M(1,1) returns row 1 and column 1 of matrix M.
    float& operator ()(int i, int j){
      return (n[j][i]);
    }

    // Index operator with two dimensions
    // Example: M(1,1) returns row 1 and column 1 of matrix M.
    const float& operator ()(int i, int j) const{
      return (n[j][i]);
    }

    // Return a single row vector from the matrix 
    Vector4f& operator [](int j){
      return (*reinterpret_cast<Vector4f *>(n[j]));
    }

    // Return a single row vector from the matrix 
    const Vector4f& operator [](int j) const{
      return (*reinterpret_cast<const Vector4f *>(n[j]));
    }

    // Make a matrix rotate about various axis
    Matrix4f MakeRotationX(float t){
        Matrix4f ret;
        ret.identity();
        ret.n[1][1] = cos(t);
        ret.n[2][1] = sin(t)*-1;
        ret.n[1][2] = sin(t);
        ret.n[2][2] = cos(t);

        return ret;
    }

    Matrix4f MakeRotationY(float t){
        Matrix4f ret;
        ret.identity();
        ret.n[0][0] = cos(t);
        ret.n[0][2] = sin(t)*-1;
        ret.n[2][0] = sin(t);
        ret.n[2][2] = cos(t);

        return ret;
    }
   
   Matrix4f MakeRotationZ(float t){
        Matrix4f ret;
        ret.identity();
        ret.n[0][0] = cos(t);
        ret.n[1][0] = sin(t)*-1;
        ret.n[0][1] = sin(t);
        ret.n[1][1] = cos(t);

        return ret;
    }
    
    Matrix4f MakeScale(float sx,float sy, float sz){
        Matrix4f ret;
        ret.identity();
        ret.n[0][0] = sx;
        ret.n[1][1] = sy;
        ret.n[2][2] = sz;

        return ret;
    }

    // Equality for testing
    bool operator==(const Matrix4f& other) {
        return (*this)[0] == other[0] &&
               (*this)[1] == other[1] &&
               (*this)[2] == other[2] &&
               (*this)[3] == other[3];
    }
};


// Matrix Multiplication
Matrix4f operator *(const Matrix4f& A, const Matrix4f& B){
  Vector4f aRow0 = Vector4f(A[0][0], A[1][0], A[2][0], A[3][0]);
  Vector4f aRow1 = Vector4f(A[0][1], A[1][1], A[2][1], A[3][1]);
  Vector4f aRow2 = Vector4f(A[0][2], A[1][2], A[2][2], A[3][2]);
  Vector4f aRow3 = Vector4f(A[0][3], A[1][3], A[2][3], A[3][3]);

  Matrix4f mat4 = Matrix4f(Dot(aRow0, B[0]), Dot(aRow0, B[1]), Dot(aRow0, B[2]), Dot(aRow0, B[3]),
      Dot(aRow1, B[0]), Dot(aRow1, B[1]), Dot(aRow1, B[2]), Dot(aRow1, B[3]),
      Dot(aRow2, B[0]), Dot(aRow2, B[1]), Dot(aRow2, B[2]), Dot(aRow2, B[3]),
      Dot(aRow3, B[0]), Dot(aRow3, B[1]), Dot(aRow3, B[2]), Dot(aRow3, B[3]));

  return mat4;
}

// Matrix multiply by a vector
Vector4f operator *(const Matrix4f& M, const Vector4f& v){
  Vector4f aRow0 = Vector4f(M[0][0], M[1][0], M[2][0], M[3][0]);
  Vector4f aRow1 = Vector4f(M[0][1], M[1][1], M[2][1], M[3][1]);
  Vector4f aRow2 = Vector4f(M[0][2], M[1][2], M[2][2], M[3][2]);
  Vector4f aRow3 = Vector4f(M[0][3], M[1][3], M[2][3], M[3][3]);

  Vector4f v4 = Vector4f(Dot(aRow0, v), Dot(aRow1, v), Dot(aRow2, v), Dot(aRow3, v));
  return v4;
}


// For Debugging!
std::ostream& operator<<(std::ostream& os, const Matrix4f& obj) {
    return os << "Matrix4f(" << obj.n[0][0] << "," << obj.n[0][1] << "," << obj.n[0][2] << "," << obj.n[0][3] << "," << " "  
                             << obj.n[1][0] << "," << obj.n[1][1] << "," << obj.n[1][2] << "," << obj.n[1][3] << "," << " "  
                             << obj.n[2][0] << "," << obj.n[2][1] << "," << obj.n[2][2] << "," << obj.n[2][3] << "," << " "  
                             << obj.n[3][0] << "," << obj.n[3][1] << "," << obj.n[3][2] << "," << obj.n[3][3] << "," << " "  << ")";
}

#endif
