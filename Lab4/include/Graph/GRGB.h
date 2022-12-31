#pragma once

#include <cstdlib>  // random

#include "Math/Vector.h"

struct GRGB {
    float r;
    float g;
    float b;
    // float alpha = 0.0f;

    inline GRGB() { this->randomRGB(); }
    inline GRGB(Vector3 vec3)
    {
        this->r = vec3.v1;
        this->g = vec3.v2;
        this->b = vec3.v3;
    }
    inline GRGB(const float r, const float g, const float b) : r(r), g(g), b(b) {}

    inline void randomRGB()
    {
        this->r = (float)rand() / (float)RAND_MAX;
        this->g = (float)rand() / (float)RAND_MAX;
        this->b = (float)rand() / (float)RAND_MAX;
    }

    inline Vector3 toVec3() const { return Vector3(this->r, this->g, this->b); }

    inline GRGB norm() const
    {
        float tmp_r = this->r;
        float tmp_g = this->g;
        float tmp_b = this->b;

        if (tmp_r > 1)
            tmp_r = 1;
        else if (tmp_r < 0)
            tmp_r = 0;

        if (tmp_g > 1)
            tmp_g = 1;
        else if (tmp_g < 0)
            tmp_g = 0;

        if (tmp_b > 1)
            tmp_b = 1;
        else if (tmp_b < 0)
            tmp_b = 0;

        return GRGB(tmp_r, tmp_g, tmp_b);
    }
};
