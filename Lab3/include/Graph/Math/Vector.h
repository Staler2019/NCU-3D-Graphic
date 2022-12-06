#pragma once

#include <cmath>
#include <iostream>

#include "Shape/Line.h"

struct Vector3 {
    float v1;
    float v2;
    float v3;

    inline Vector3() { this->v1 = this->v2 = this->v3 = 0.0f; }

    inline Vector3(float v1, float v2, float v3) : v1(v1), v2(v2), v3(v3) {}

    inline void operator+=(const Vector3 &that) {
        this->v1 += that.v1;
        this->v2 += that.v2;
        this->v3 += that.v3;
    }

    inline void operator-=(const Vector3 &that) {
        this->v1 -= that.v1;
        this->v2 -= that.v2;
        this->v3 -= that.v3;
    }

    inline void operator*=(const float &that) {
        this->v1 *= that;
        this->v2 *= that;
        this->v3 *= that;
    }

    inline void operator/=(const float &that) {
        this->v1 /= that;
        this->v2 /= that;
        this->v3 /= that;
    }

    inline void operator==(const float &that) const {
        return ((this->v1 == that.v1) && (this->v2 == that.v2) && (this->v3 == that.v3));
    }

    inline bool operator!=(const Vector3 &that) const {
        return !(*this == that);
    }

    inline Vector3 operator+(const Vector3 &that) const {
        return Vector3(this->v1 + that.v1, this->v2 + that.v2, this->v3 + that.v3);
    }

    inline Vector3 operator-(const Vector3 &that) const {
        return Vector3(this->v1 - that.v1, this->v2 - that.v2, this->v3 - that.v3);
    }

    inline Vector3 operator*(const float that) const {
        return Vector3(this->v1 * that, this->v2 * that, this->v3 * that);
    }

    inline Vector3 operator/(const float that) const {
        return Vector3(this->v1 / that, this->v2 / that, this->v3 / that);
    }

    inline Vector3 dot(const Vector3 &that) const {
        return Vector3(this->v1 * that.v1, this->v2 * that.v2, this->v3 * that.v3);
    }

    inline Vector3 cross(const Vector3 &that) const {
        return Vector3(this->v2 * that.v3 - this->v3 * that.v2, this->v3 * that.v1 - this->v1 * that.v3,
                       this->v1 * that.v2 - this->v2 * that.v1);
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector3& that);

    inline Vector3 norm() const {
        float length = this->len();
        return *this / length;
    }

    inline Vector3 inverse() const { return Vector3(-this->v1, -this->v2, -this->v3); }

    inline float len() const { return (sqrt(pow(this->v1, 2) + pow(this->v2, 2) + pow(this->v3, 2))); }
};
