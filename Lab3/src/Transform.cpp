#include "Transform.h"

#include <cmath>
#include <iostream>

// #include "Shape/Point.h"
#include "Math/Matrix.h"
#include "Math/Util.h"
#include "Math/Vector.h"

Transform3D::Transform3D() : Matrix4() {}

Transform3D::Transform3D(const float that[4][4]) : Matrix4(that) {}

Transform3D::Transform3D(const Vector3& vec1, const Vector3& vec2,
                         const Vector3& vec3)
    : Matrix4(vec1, vec2, vec3)
{
}

void Transform3D::reset() { this->ones(); }

void Transform3D::print() { std::cerr << *this; }

void Transform3D::scale(Vector3 vec, bool printM)
{
    Matrix4 tmp_sc;

    tmp_sc.m[0][0] = vec.v1;
    tmp_sc.m[1][1] = vec.v2;
    tmp_sc.m[2][2] = vec.v3;

    *this = tmp_sc * (*this);

    if (printM) {
        std::cerr << "Scale: " << vec << "\n";
        this->print();
    }
}

void Transform3D::rotate(Vector3 vec_deg, bool printM)
{
    Matrix4 tmp_deg;  // TODO.
    // tmp_deg.ones(); // in constructor

    // y // TODO.error?
    const float r2 = toRad(vec_deg.v2);

    tmp_deg.m[0][0] = cos(r2);
    tmp_deg.m[0][2] = sin(r2);
    tmp_deg.m[2][0] = -sin(r2);
    tmp_deg.m[2][2] = cos(r2);

    *this = tmp_deg * (*this);

    // z
    tmp_deg.ones();
    const float r3 = toRad(vec_deg.v3);

    tmp_deg.m[0][0] = cos(r3);
    tmp_deg.m[0][1] = -sin(r3);
    tmp_deg.m[1][0] = sin(r3);
    tmp_deg.m[1][1] = cos(r3);

    *this = tmp_deg * (*this);

    // x
    tmp_deg.ones();
    const float r1 = toRad(vec_deg.v1);

    tmp_deg.m[1][1] = cos(r1);
    tmp_deg.m[1][2] = -sin(r1);
    tmp_deg.m[2][1] = sin(r1);
    tmp_deg.m[2][2] = cos(r1);

    *this = tmp_deg * (*this);

    if (printM) {
        std::cerr << "Rotate: " << vec_deg << "\n";
        this->print();
    }
}

void Transform3D::translate(Vector3 vec, bool printM)
{
    Matrix4 tmp_tr;

    tmp_tr.m[0][3] += vec.v1;
    tmp_tr.m[1][3] += vec.v2;
    tmp_tr.m[2][3] += vec.v3;

    *this = tmp_tr * (*this);

    if (printM) {
        std::cerr << "Translate: " << vec << "\n";
        this->print();
    }
}

Vector3 Transform3D::getResult(const Vector3& vec) const
{
    const float& tmp_v1 = vec.v1;
    const float& tmp_v2 = vec.v2;
    const float& tmp_v3 = vec.v3;

    return Vector3(this->m[0][0] * tmp_v1 + this->m[0][1] * tmp_v2 +
                       this->m[0][2] * tmp_v3 + this->m[0][3],
                   this->m[1][0] * tmp_v1 + this->m[1][1] * tmp_v2 +
                       this->m[1][2] * tmp_v3 + this->m[1][3],
                   this->m[2][0] * tmp_v1 + this->m[2][1] * tmp_v2 +
                       this->m[2][2] * tmp_v3 + this->m[2][3]);
}

Vector4 Transform3D::getResult(const Vector4& vec) const
{
    const float& tmp_v1 = vec.v1;
    const float& tmp_v2 = vec.v2;
    const float& tmp_v3 = vec.v3;
    const float& tmp_v4 = vec.v4;

    return Vector4(this->m[0][0] * tmp_v1 + this->m[0][1] * tmp_v2 +
                       this->m[0][2] * tmp_v3 + this->m[0][3] * tmp_v4,
                   this->m[1][0] * tmp_v1 + this->m[1][1] * tmp_v2 +
                       this->m[1][2] * tmp_v3 + this->m[1][3] * tmp_v4,
                   this->m[2][0] * tmp_v1 + this->m[2][1] * tmp_v2 +
                       this->m[2][2] * tmp_v3 + this->m[2][3] * tmp_v4,
                   this->m[3][0] * tmp_v1 + this->m[3][1] * tmp_v2 +
                       this->m[3][2] * tmp_v3 + this->m[3][3] * tmp_v4);
}

void Transform3D::operator=(const Matrix4& that)
{
    *this = Transform3D(that.m);
}

Transform3D Transform3D::operator*(
    const Transform3D& that) const  // same as Matrix4
{
    float m_this[4][4] = {};

    // progress
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            for (int k = 0; k < 4; k++)
                m_this[i][j] += this->m[i][k] * that.m[k][j];

    return Transform3D(m_this);
}

/********Transform2D********/

void Transform2D::matMul(float m_that[3][3])
{
    float m_this[3][3] = {};

    // progress
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            for (int k = 0; k < 3; k++)
                m_this[i][j] += m_that[i][k] * this->m[k][j];

    // copy
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++) this->m[i][j] = m_this[i][j];
    return;
}

void Transform2D::printMat()
{
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) std::cerr << this->m[i][j] << ", ";
        std::cerr << std::endl;
    }

    return;
}

Transform2D::Transform2D()
{
    this->reset();
    return;
}

void Transform2D::reset()
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

void Transform2D::scale(Vector2 vec, bool printM /*=1*/)
{
    /*
        |x_way   0     0   |*|m00 m01 m02|
        |  0   y_way   0   | |m10 m11 m12|
        |  0     0     1   | |m20 m21 m22|
    */
    float m_sc[3][3] = {};
    m_sc[0][0] = vec.v1;
    m_sc[1][1] = vec.v2;
    m_sc[2][2] = 1;

    this->matMul(m_sc);
    if (printM) this->printMat();

    return;
}

void Transform2D::rotate(float degree, bool printM /*=1*/)
{
    /*
        - counterclockwise
            |cos -sin   0 |*|m00 m01 m02|
            |sin  cos   0 | |m10 m11 m12|
            | 0    0    1 | |m20 m21 m22|
    */
    const float rad = toRad(degree);

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

void Transform2D::translate(Vector2 vec, bool printM /*=1*/)
{
    this->m[0][2] += vec.v1;
    this->m[1][2] += vec.v2;
    if (printM) this->printMat();

    return;
}

Point Transform2D::getResult(Point p) const
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
