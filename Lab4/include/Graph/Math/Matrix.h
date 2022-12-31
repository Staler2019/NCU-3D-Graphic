#pragma once

#include <iostream>

#include "Math/Vector.h"

struct Matrix2 {
    /*
        |m00 m01|
        |m10 m11|
    */
    bool exist = true;

    float m[2][2]{};

    Matrix2();

    Matrix2(const float that[2][2]);

    Matrix2(const float m00, const float m01, const float m10,
                   const float m11);

    void ones();

    void zeros();

    Matrix2 inverse() const;

    inline bool isExist() const { return this->exist; }

    Vector2 operator*(const Vector2& that) const;
};

struct Matrix4 {
    /*
        |m00 m01 m02 m03|
        |m10 m11 m12 m13|
        |m20 m21 m22 m23|
        |m30 m31 m32 m33|
    */
    float m[4][4]{};

    Matrix4();
    Matrix4(const float that[4][4]);
    inline Matrix4(const Vector3& vec1, const Vector3& vec2,
                   const Vector3& vec3)
    {
        this->m[0][0] = vec1.v1;
        this->m[0][1] = vec1.v2;
        this->m[0][2] = vec1.v3;
        this->m[0][3] = 0.0f;
        this->m[1][0] = vec2.v1;
        this->m[1][1] = vec2.v2;
        this->m[1][2] = vec2.v3;
        this->m[1][3] = 0.0f;
        this->m[2][0] = vec3.v1;
        this->m[2][1] = vec3.v2;
        this->m[2][2] = vec3.v3;
        this->m[2][3] = 0.0f;
        this->m[3][0] = 0.0f;
        this->m[3][1] = 0.0f;
        this->m[3][2] = 0.0f;
        this->m[3][3] = 1.0f;
    }

    void zeros();
    void ones();

    Matrix4 operator*(const Matrix4& that);

    friend std::ostream& operator<<(std::ostream& os, const Matrix4& that);
};
