#pragma once

#include <cstdlib>  // random

struct GRGB {
    float r;
    float g;
    float b;
    // float alpha = 0.0f;

    inline GRGB() { this->randomRGB(); }
    inline GRGB(const float r, const float g, const float b) : r(r), g(g), b(b) {}

    inline void randomRGB()
    {
        this->r = (float)rand() / (float)RAND_MAX;
        this->g = (float)rand() / (float)RAND_MAX;
        this->b = (float)rand() / (float)RAND_MAX;
    }
};

