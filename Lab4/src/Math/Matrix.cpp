#include "Math/Matrix.h"

#include <iostream>

Matrix2::Matrix2() { this->ones(); }

Matrix2::Matrix2(const float that[2][2])
{
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++) this->m[i][j] = that[i][j];
}

Matrix2::Matrix2(const float m00, const float m01, const float m10,
                   const float m11){
    this->m[0][0] = m00;
    this->m[0][1] = m01;
    this->m[1][0] = m10;
    this->m[1][1] = m11;
}

void Matrix2::ones()
{
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++) this->m[i][j] = (i == j) ? 1 : 0;
}

void Matrix2::zeros()
{
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++) this->m[i][j] = 0;
}

Matrix2 Matrix2::inverse() const
{
    float det = this->m[0][0] * this->m[1][1] - this->m[0][1] * this->m[1][0];

    if (det == 0) {
        Matrix2 mat2;
        mat2.exist = false;
        return mat2;
    }

    float tmp_m[2][2];
    tmp_m[0][0] = this->m[1][1] / det;
    tmp_m[0][1] = -this->m[0][1] / det;
    tmp_m[1][0] = -this->m[1][0] / det;
    tmp_m[1][1] = this->m[0][0] / det;

    return Matrix2(tmp_m);
}

Vector2 Matrix2::operator*(const Vector2& that) const
{
    return Vector2(this->m[0][0] * that.v1 + this->m[0][1] * that.v2,
                   this->m[1][0] * that.v1 + this->m[1][1] * that.v2);
}

Matrix4::Matrix4() { this->ones(); }

Matrix4::Matrix4(const float that[4][4])
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) this->m[i][j] = that[i][j];
}

void Matrix4::zeros()
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) this->m[i][j] = 0;
}

void Matrix4::ones()
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) this->m[i][j] = (i == j) ? 1 : 0;
}

Matrix4 Matrix4::operator*(const Matrix4& that)
{
    float m_this[4][4] = {};

    // progress
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            for (int k = 0; k < 4; k++)
                m_this[i][j] += this->m[i][k] * that.m[k][j];

    return Matrix4(m_this);
}

std::ostream& operator<<(std::ostream& os, const Matrix4& that)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) os << that.m[i][j] << ", ";
        os << std::endl;
    }
    return os;
}
