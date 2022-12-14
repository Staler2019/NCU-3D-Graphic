#include "Math/Matrix.h"

#include <iostream>

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
            for (int k = 0; k < 4; k++) m_this[i][j] += this->m[i][k] * that.m[k][j];

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
