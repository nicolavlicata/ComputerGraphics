// Includes for the assignment
#include "Vector4f.h"
#include "Matrix4f.h"
#include <iostream>

// Tests for comparing our library
// You may compare your operations against the glm library
// which is a well tested library.
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/projection.hpp>

#include "util.h"

// Compare my Matrix type with the glm implementation.
bool Matrix4fEqualsMat4(Matrix4f& myMatrix, glm::mat4& glmMatrix) {
    return (
        fcomp(glmMatrix[0][0], myMatrix[0][0]) &&
        fcomp(glmMatrix[0][1], myMatrix[0][1]) &&
        fcomp(glmMatrix[0][2], myMatrix[0][2]) &&
        fcomp(glmMatrix[0][3], myMatrix[0][3]) &&
        fcomp(glmMatrix[1][0], myMatrix[1][0]) &&
        fcomp(glmMatrix[1][1], myMatrix[1][1]) &&
        fcomp(glmMatrix[1][2], myMatrix[1][2]) &&
        fcomp(glmMatrix[1][3], myMatrix[1][3]) &&
        fcomp(glmMatrix[2][0], myMatrix[2][0]) &&
        fcomp(glmMatrix[2][1], myMatrix[2][1]) &&
        fcomp(glmMatrix[2][2], myMatrix[2][2]) &&
        fcomp(glmMatrix[2][3], myMatrix[2][3]) &&
        fcomp(glmMatrix[3][0], myMatrix[3][0]) &&
        fcomp(glmMatrix[3][1], myMatrix[3][1]) &&
        fcomp(glmMatrix[3][2], myMatrix[3][2]) &&
        fcomp(glmMatrix[3][3], myMatrix[3][3])
    );
}

bool Vector4fEqualsVec4(Vector4f& myVec, glm::vec4& glmVec) {
    return (
        fcomp(glmVec[0], myVec[0]),
        fcomp(glmVec[1], myVec[1]),
        fcomp(glmVec[2], myVec[2]),
        fcomp(glmVec[3], myVec[3])
    );
}


// Sample unit test comparing against GLM.
bool unitTest0(){
    glm::mat4 glmIdentityMatrix = glm::mat4(1.0f);
    Matrix4f myIdentity( 1.0f,0,0,0,
                         0,1.0f,0,0,
                         0,0,1.0f,0,
                         0,0,0,1.0f);
    return Matrix4fEqualsMat4(myIdentity, glmIdentityMatrix);
}

bool unitTest1(){
    glm::mat4 glmIdentityMatrix = glm::mat4(1.0f);
    Matrix4f myIdentity( 1.0f,0,0,0,
                         0,1.0f,0,0,
                         0,0,1.0f,0,
                         0,0,0,1.0f);
    return Matrix4fEqualsMat4(myIdentity, glmIdentityMatrix);
}

// Sample unit test comparing against GLM.
bool unitTest2(){
    glm::mat4 glmIdentityMatrix = glm::mat4(1.0f);
    Vector4f a(1.0f ,0.0f,  0.0f,   0.0f);
    Vector4f b(0.0f ,1.0f,  0.0f,   0.0f);
    Vector4f c(0.0f ,0.0f,  1.0f,   0.0f);
    Vector4f d(0.0f ,0.0f,  0.0f,   1.0f);
    Matrix4f myIdentity(a,b,c,d);

    Matrix4f identity2(a,a,a,a);
    identity2.identity();
    bool assert1 = myIdentity == identity2;

    return assert1 && Matrix4fEqualsMat4(identity2, glmIdentityMatrix);
}

// Sample unit test comparing against GLM.
bool unitTest3(){
    glm::vec3 scales = glm::vec3(2.0f, 2.0f, 2.0f);
    glm::mat4 glmScale = glm::scale(scales);
    Vector4f a(1.0f,0,0,0);
    Vector4f b(0.0f,1.0f,0,0);
    Vector4f c(0,0,1.0f,0);
    Vector4f d(0,0,0,1.0f);
    Matrix4f myIdentity(a,b,c,d);
    Matrix4f myScaled = myScaled.MakeScale(2.0f,2.0f,2.0f);

    return Matrix4fEqualsMat4(myScaled, glmScale);
}

// Sample unit test comparing against GLM.
// Testing operator
bool unitTest4(){
    glm::mat4 glmTest = glm::mat4(1.0f);
    glmTest[1][3] = 72.0f;
    glmTest[2][3] = 2.1f;

    Matrix4f myMatrix(1,0,0,0,
                      0,1,0,0,
                      0,0,1,0,
                      0,0,0,1);

    myMatrix[1][3] = 72.0f;
    myMatrix[2][3] = 2.1f;
    
    return Matrix4fEqualsMat4(myMatrix, glmTest);
}

// Sample unit test testing your library
bool unitTest5(){
  Vector4f a(1,1,1,1);
  Vector4f b(0,0,0,0);
  Vector4f c = a + b;

  if(c.x == 1 && c.y == 1 && c.z ==1 && c.w==1){
    return true;
  }
    return false;
}


/*
    VECTOR TESTS
*/

bool vecTestConstructor() {
    Vector4f v = Vector4f(1,2,3,4);
    bool assert1 = v[0] == 1 && v[1] == 2 && v[2] == 3 && v[3] == 4;

    bool assert2 = Vector4f(0,0.0199987,0.9998,0) == Vector4f(0,0.0199987,0.9998,0);

    return assert1 && assert2;
}

bool vecTestScalarMultiply() {
    Vector4f v = Vector4f(1,2,3,4);
    Vector4f v1 = Vector4f(1,2,3,4);
    bool assert1 = v == v1;
    
    v *= 1;
    bool assert2 = v == v1;

    v1 *= 2;
    bool assert3 = !(v == v1);
    bool assert7 = !(v == v1);

    bool assert4 = (v *= 4) == (v1 *= 2);

    return assert1 && assert2 && assert3 && assert4;
}

bool vecTestScalarDivide() {
    Vector4f v = Vector4f(12, 24, 60, 0);
    Vector4f v1 = Vector4f(12, 24, 60, 0);
    
    bool assert1 = v == v1;
    
    v /= 1;
    bool assert2 = v == v1;

    v1 /= 2;
    bool assert3 = !(v == v1);
    bool assert4 = v1 == Vector4f(6,12,30,0);

    v /= 5;
    bool assert5 = v == Vector4f(2.4,4.8,12,0);

    return assert1 && assert2 && assert3 && assert4 && assert5;
}

bool vecTestPlusMinus() {
    Vector4f v = Vector4f(0,0,0,0);
    Vector4f v1 = Vector4f(1,0,1,0);
    Vector4f v2 = Vector4f(4,4,7,67);
    Vector4f v3 = Vector4f(2,0,2,0);

    bool assert1 = v == (v + v);
    bool assert2 = v1 == (v1 + v);

    bool assert3 = (v1 + v1) == v3;
    bool assert4 = (v3 - v1) == v1;
    bool assert5 = ((v3 - v1) - v1) == v;

    bool assert6 = (v1 + v2 - v1) == v2;
    bool assert7 = (v3 - v2) == Vector4f(-2,-4,-5,-67);

    return assert1 && assert2 && assert3 && assert4 && assert5 && assert6 && assert7;
}

bool vecTestDotProduct() {
    Vector4f v = Vector4f(0,0,0,0);
    Vector4f v1 = Vector4f(1,1,1,1);
    Vector4f v2 = Vector4f(1,2,3,4);

    bool assert1 = Dot(v, v) == 0;
    bool assert2 = Dot(v1, v) == 0;
    bool assert3 = Dot(v, v2) == 0;
    bool assert4 = Dot(v1, v1) == 4;
    bool assert5 = Dot(v2, v2) == (1+4+9+16);

    return assert1 && assert2 && assert3 && assert4 && assert5;
}

bool vecTestMagnitude() {
    Vector4f v0 = Vector4f(0,0,0,0); 
    Vector4f v1 = Vector4f(1,1,1,1); 
    
    bool assert0 = Magnitude(v0) == 0;
    bool assert1 = Magnitude(v1) == 2;
    
    return assert0 && assert1;
}

bool vecTestNormalize() {
    Vector4f v0 = Vector4f(1, 1, 1, 1);
    Vector4f vn = Normalize(v0);
    
    bool assert0 = Magnitude(v0) != 1;
    bool assert1 = Magnitude(vn) == 1;
}

bool vecTestProjection() {
    Vector4f v0(2, 2, 2, 1);
    Vector4f v1(1, 1, 2, 1);
    Vector4f vp = Project(v0, v1);

    glm::vec4 glv0(2, 2, 2, 1);
    glm::vec4 glv1(1, 1, 2, 1);
    glm::vec4 glvp = glm::proj(glv0, glv1);

    return Vector4fEqualsVec4(vp, glvp); 
}

bool vecTestCrossProduct() {
    Vector4f v0(1, 2, 2, 1);
    Vector4f v1(3, 4, 2, 1);
    Vector4f vcp = CrossProduct(v0, v1);

    glm::vec3 glv0(1, 2, 2); 
    glm::vec3 glv1(3, 4, 2); 
    glm::vec3 glvcp = glm::cross(glv0, glv1);
    glm::vec4 glCompare(glvcp[0], glvcp[1],glvcp[2],1);
    
    return Vector4fEqualsVec4(vcp, glCompare);
}


/*
    MATRIX TESTS
*/

bool matTestRotation() {
    Matrix4f m1;
    m1.identity();

    // radians
    float xAngle = 0.02f;
    float yAngle = 1;
    float zAngle = 17;

    Matrix4f mRotX = Matrix4f(1,0,0,0, 0,0.9998,-0.0199987,0, 0,0.0199987,0.9998,0, 0,0,0,1);
    Matrix4f mRotY = Matrix4f(0.540302,0,0.841471,0, 0,1,0,0, -0.841471,0,0.540302,0, 0,0,0,1);
    Matrix4f mRotZ = Matrix4f(-0.275163,0.961397,0,0, -0.961397,-0.275163,0,0, 0,0,1,0, 0,0,0,1);

    glm::mat4 glmRotX = rotate(xAngle, glm::vec3(1.0, 0.0, 0.0));
    glm::mat4 glmRotY = rotate(yAngle, glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 glmRotZ = rotate(zAngle, glm::vec3(0.0, 0.0, 1.0));

    Matrix4f actualRotX = m1.MakeRotationX(xAngle);
    Matrix4f actualRotY = m1.MakeRotationY(yAngle);
    Matrix4f actualRotZ = m1.MakeRotationZ(zAngle);

    return actualRotX == mRotX && Matrix4fEqualsMat4(actualRotX, glmRotX) &&
           actualRotY == mRotY && Matrix4fEqualsMat4(actualRotY, glmRotY) &&
           actualRotZ == mRotZ && Matrix4fEqualsMat4(actualRotZ, glmRotZ);
}

bool matTestMatrixMultiply() {
    Matrix4f myId; 
    myId.identity();

    Matrix4f zeros = Matrix4f();

    Matrix4f mRotY = Matrix4f(0.540302,0,0.841471,0, 0,1,0,0, -0.841471,0,0.540302,0, 0,0,0,1);
    Matrix4f mScale = myId.MakeScale(2.0f,2.0f,2.0f);

    glm::mat4 glmRotY = rotate(1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 glmScale = glm::scale(glm::vec3(2.0f, 2.0f, 2.0f));
    glm::mat4 glmRotYScale = glmRotY * glmScale;
    
    bool assert1 = (myId * myId) == myId;
    bool assert2 = (myId * mRotY) == mRotY;
    bool assert3 = (myId * zeros) == zeros;
    bool assert4 = (zeros * zeros) == zeros;

    Matrix4f expectedRotYScale = Matrix4f(1.0806,0,1.68294,0, 0,2,0,0, -1.68294,0,1.0806,0, 0,0,0,1);

    bool assert5 = expectedRotYScale == (mRotY * mScale);
    bool assert6 = Matrix4fEqualsMat4(expectedRotYScale, glmRotYScale);

    return assert1 && assert2 && assert3 && assert4 && assert5 && assert6;
}

bool matTestMatrixProjection() {
    // My Library
    Matrix4f myId; 
    myId.identity();

    Matrix4f zeros = Matrix4f();

    Matrix4f mRotY = Matrix4f(0.540302,0,0.841471,0, 0,1,0,0, -0.841471,0,0.540302,0, 0,0,0,1);
    Matrix4f mScale = myId.MakeScale(2.0f,2.0f,2.0f);

    Vector4f zero = Vector4f(0,0,0,0);
    Vector4f one = Vector4f(1,1,1,1);
    Vector4f interesting  = Vector4f(2,0,1,1);

    bool assert1 = (zeros * zero) == zero;
    bool assert2 = (myId * zero) == zero;
    bool assert3 = (myId * one) == one;
    bool assert4 = (zeros * one) == zero;
    bool assert5 = (mScale * one) == Vector4f(2,2,2,1);
    bool assert6 = (mRotY * interesting) == Vector4f(1.92208,0,-1.14264,1);

    // associative
    bool assert7 = ((mRotY * mScale) * interesting) == (mRotY * (mScale * interesting));

    // glm library
    glm::mat4 glmRotY = rotate(1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 glmScale = glm::scale(glm::vec3(2.0f, 2.0f, 2.0f));
    glm::mat4 glmRotYScale = glmRotY * glmScale;

    glm::vec4 glmZero = glm::vec4(0.0f,0.0f,0.0f,0.0f);
    glm::vec4 glmOne = glm::vec4(1.0f,1.0f,1.0f,1.0f);
    glm::vec4 glmInteresting  = glm::vec4(2.0f,0.0f,1.0f,1.0f);

    bool assert8 = (glmScale * glmOne) == glm::vec4(2.0f,2.0f,2.0f,1.0f);
    glm::vec4 interestingRotation = glmRotY * glmInteresting;
    bool assert9 = fcomp(interestingRotation[0], 1.92208f) &&
                   fcomp(interestingRotation[1], 0.0f) &&
                   fcomp(interestingRotation[2], -1.14264f) &&
                   fcomp(interestingRotation[3], 1.0f);

    return assert1 && assert2 && assert3 && assert4 && assert5 && assert6 && assert7 && assert8 && assert9; 
}

int main(){
    // Keep track of the tests passed
    unsigned int testsPassed = 0;

    // Run 'unit tests'
    std::cout << "Passed 0: " << unitTest0() << " \n";
    std::cout << "Passed 1: " << unitTest1() << " \n";
    std::cout << "Passed 2: " << unitTest2() << " \n";
    std::cout << "Passed 3: " << unitTest3() << " \n";
    std::cout << "Passed 4: " << unitTest4() << " \n";
    std::cout << "Passed 5: " << unitTest5() << " \n";
    
    std::cout << "Passed vec: " << vecTestConstructor() << " \n";
    std::cout << "Passed vec: " << vecTestScalarMultiply() << " \n";
    std::cout << "Passed vec: " << vecTestScalarDivide() << " \n";
    std::cout << "Passed vec: " << vecTestPlusMinus() << " \n";
    std::cout << "Passed vec: " << vecTestDotProduct() << " \n";
    std::cout << "Passed vec: " << vecTestMagnitude() << " \n";
    std::cout << "Passed vec: " << vecTestNormalize() << " \n";
    std::cout << "Passed vec: " << vecTestProjection() << " \n";
    std::cout << "Passed vec: " << vecTestCrossProduct() << " \n";

    std::cout << "Passed mat: " << matTestRotation() << " \n";
    std::cout << "Passed mat: " << matTestMatrixMultiply() << " \n";
    std::cout << "Passed mat: " << matTestMatrixProjection() << " \n";

    return 0;
}
