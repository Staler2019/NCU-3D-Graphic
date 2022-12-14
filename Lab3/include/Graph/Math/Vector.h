#pragma once

#include <cmath>
#include <iostream>

struct Vector3 {
    float v1;
    float v2;
    float v3;

    inline Vector3() { this->v1 = this->v2 = this->v3 = 0.0f; }

    inline Vector3(float v1, float v2, float v3) : v1(v1), v2(v2), v3(v3) {}

    inline void operator+=(const Vector3 &that)
    {
        this->v1 += that.v1;
        this->v2 += that.v2;
        this->v3 += that.v3;
    }

    inline void operator-=(const Vector3 &that)
    {
        this->v1 -= that.v1;
        this->v2 -= that.v2;
        this->v3 -= that.v3;
    }

    inline void operator*=(const float &that)
    {
        this->v1 *= that;
        this->v2 *= that;
        this->v3 *= that;
    }

    inline void operator/=(const float &that)
    {
        this->v1 /= that;
        this->v2 /= that;
        this->v3 /= that;
    }

    inline bool operator==(const Vector3 &that) const
    {
        return ((this->v1 == that.v1) && (this->v2 == that.v2) &&
                (this->v3 == that.v3));
    }

    inline bool operator!=(const Vector3 &that) const
    {
        return !(*this == that);
    }

    inline Vector3 operator+(const Vector3 &that) const
    {
        return Vector3(this->v1 + that.v1, this->v2 + that.v2,
                       this->v3 + that.v3);
    }

    inline Vector3 operator-(const Vector3 &that) const
    {
        return Vector3(this->v1 - that.v1, this->v2 - that.v2,
                       this->v3 - that.v3);
    }

    inline Vector3 operator*(const float that) const
    {
        return Vector3(this->v1 * that, this->v2 * that, this->v3 * that);
    }

    inline Vector3 operator/(const float that) const
    {
        return Vector3(this->v1 / that, this->v2 / that, this->v3 / that);
    }

    inline float dot(const Vector3 &that) const
    {
        return (this->v1 * that.v1 + this->v2 * that.v2 + this->v3 * that.v3);
    }

    inline Vector3 cross(const Vector3 &that) const
    {
        return Vector3(this->v2 * that.v3 - this->v3 * that.v2,
                       this->v3 * that.v1 - this->v1 * that.v3,
                       this->v1 * that.v2 - this->v2 * that.v1);
    }

    friend std::ostream &operator<<(std::ostream &os, const Vector3 &that);

    inline Vector3 norm() const
    {
        float length = this->len();
        return *this / length;
    }

    inline Vector3 inverse() const
    {
        return Vector3(-this->v1, -this->v2, -this->v3);
    }

    inline float len() const
    {
        return (sqrt(pow(this->v1, 2) + pow(this->v2, 2) + pow(this->v3, 2)));
    }
};

struct Vector4 {
    float v1;
    float v2;
    float v3;
    float v4;

    inline Vector4() { this->v1 = this->v2 = this->v3 = this->v4 = 0.0f; }
    inline Vector4(float v1, float v2, float v3, float v4)
        : v1(v1), v2(v2), v3(v3), v4(v4)
    {
    }
    inline Vector4(const Vector3 &vec3, float v4) // v4 = 0 is a direction vector
        : v1(vec3.v1), v2(vec3.v2), v3(vec3.v3), v4(v4)
    {
    }

    inline void operator+=(const Vector4 &that)
    {
        this->v1 += that.v1;
        this->v2 += that.v2;
        this->v3 += that.v3;
        this->v4 += that.v4;
    }
    inline Vector4 operator*(const float that) const
    {
        return Vector4(this->v1 * that, this->v2 * that, this->v3 * that,
                       this->v4 * that);
    }
    inline Vector4 operator-(const Vector4 &that) const
    {
        return Vector4(this->v1 - that.v1, this->v2 - that.v2,
                       this->v3 - that.v3, this->v4 - that.v4);
    }

    friend std::ostream &operator<<(std::ostream &os, const Vector4 &that);
};

struct Vector2 {  // TODO. same as class Shape/Point
    float v1;
    float v2;

    inline Vector2() { this->v1 = this->v2 = 0.0f; }
    inline Vector2(float v1, float v2) : v1(v1), v2(v2) {}
};
