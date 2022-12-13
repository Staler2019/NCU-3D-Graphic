#pragma once

#include "Math/Matrix.h"
#include "Math/Vector.h"
#include "Shape/Point.h"

class Transform3D : protected Matrix4 {
   private:
    void operator=(const Matrix4& that);

   public:
    Transform3D();
    Transform3D(const float that[4][4]);
    Transform3D(const Vector3& vec1, const Vector3& vec2, const Vector3& vec3);

    void print();
    void reset();
    void scale(Vector3 vec, bool printM = 1);
    void rotate(Vector3 vec_deg, bool printM = 1);
    void translate(Vector3 vec, bool printM = 1);

    Vector3 getResult(const Vector3& vec) const;
    Vector4 getResult(const Vector4& vec) const;
    Transform3D operator*(const Transform3D& that) const;
};

// class Transform2D : public Matrix3 {};
class Transform2D {
    /*
        ref:
       https://www.796t.com/content/1546619583.html?fbclid=IwAR1HlZK3kmqdCC5nccQLMjTVgb5W8pRnO7Ur0GV6pRHr6THiD70Bw7KYbL0
        |m00 m01 m02|
        |m10 m11 m12|
        |m20 m21 m22|
    */
    float m[3][3] = {};

    virtual void matMul(float m_that[3][3]);

   public:
    Transform2D();

    void reset();
    void scale(Vector2 vec, bool printM = 1);
    void rotate(float degree, bool printM = 1);
    void translate(Vector2 vec, bool printM = 1);
    void printMat();

    Point getResult(Point) const;
};

// #define TRANS_SIZE 4

// class TransformMatrix3D {
//     /*
//         |m00 m01 m02 m03|
//         |m10 m11 m12 m13|
//         |m20 m21 m22 m23|
//         |m30 m31 m32 m33|
//     */
//     float m[TRANS_SIZE][TRANS_SIZE] = {};

//     void matMul(float m_that[TRANS_SIZE][TRANS_SIZE]);

//    public:
//     TransformMatrix3D();

//     void reset();
//     void scale(float x_way, float y_way, float z_way, bool printM = 1);
//     void rotate(float degree, bool printM = 1);
//     void translate(float del_x, float del_y, float del_z, bool printM = 1);
//     void printMat();
// };

// #undef TRANS_SIZE
