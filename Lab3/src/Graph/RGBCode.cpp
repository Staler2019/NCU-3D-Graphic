#include "RGBCode.h"

#include <cstdlib>  // random

RGBCode::RGBCode()
{
    this->randomRGB();
    return;
}

RGBCode::RGBCode(float r, float g, float b) : r(r), g(g), b(b) { return; }

void RGBCode::randomRGB()
{
    this->r = (float)rand() / (float)RAND_MAX;
    this->g = (float)rand() / (float)RAND_MAX;
    this->b = (float)rand() / (float)RAND_MAX;
    return;
}

float RGBCode::getR() { return this->r; }

float RGBCode::getG() { return this->g; }

float RGBCode::getB() { return this->b; }
