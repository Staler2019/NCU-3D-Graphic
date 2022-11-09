#include "TransformMatrix.h"

#define _USE_MATH_DEFINES  // cmath M_PI

#include <cmath>
#include <iostream>

#include "Point.h"

void TransformMatrix::matMul(float m_that[3][3])
{
    float m_this[3][3] = {};

    // progress
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            for (int k = 0; k < 3; k++) m_this[i][j] += m_that[i][k] * this->m[k][j];

    // copy
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++) this->m[i][j] = m_this[i][j];

    return;
}

void TransformMatrix::printMat()
{
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) std::cerr << this->m[i][j] << ", ";
        std::cerr << std::endl;
    }

    return;
}

TransformMatrix::TransformMatrix()
{
    this->reset();
    return;
}

void TransformMatrix::reset()
{
    /*
        |1 0 0|
        |0 1 0|
        |0 0 1|
    */
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++) this->m[i][j] = (i == j) ? 1.0f : 0.0f;

    return;
}

void TransformMatrix::scale(float x_way, float y_way, bool printM /*=1*/)
{
    /*
        |x_way   0     0   |*|m00 m01 m02|
        |  0   y_way   0   | |m10 m11 m12|
        |  0     0     1   | |m20 m21 m22|
    */
    float m_sc[3][3] = {};
    m_sc[0][0] = x_way;
    m_sc[1][1] = y_way;
    m_sc[2][2] = 1;

    this->matMul(m_sc);
    if (printM) this->printMat();

    return;
}

void TransformMatrix::rotate(float degree, bool printM /*=1*/)
{
    /*
        - counterclockwise
            |cos -sin   0 |*|m00 m01 m02|
            |sin  cos   0 | |m10 m11 m12|
            | 0    0    1 | |m20 m21 m22|
    */
    const float rad = degree * M_PI / 180.0f;

    float m_deg[3][3] = {};

    m_deg[0][0] = cos(rad);
    m_deg[0][1] = -sin(rad);
    m_deg[1][0] = sin(rad);
    m_deg[1][1] = cos(rad);
    m_deg[2][2] = 1.0f;

    this->matMul(m_deg);
    if (printM) this->printMat();

    return;
}

void TransformMatrix::translate(float del_x, float del_y, bool printM /*=1*/)
{
    this->m[0][2] += del_x;
    this->m[1][2] += del_y;
    if (printM) this->printMat();

    return;
}

Point TransformMatrix::getResult(Point p)
{
    /*
            |m00 m01 m02|*|x| |m00*x+m01*y+m02|
            |m10 m11 m12| |y|=|m10*x+m11*y+m12|
            |... ... ...| |1| |      ...      |
    */
    float x = p.getX();
    float y = p.getY();

    return Point(this->m[0][0] * x + this->m[0][1] * y + this->m[0][2],
                 this->m[1][0] * x + this->m[1][1] * y + this->m[1][2]);
}
