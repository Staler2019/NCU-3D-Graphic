#pragma once

class RGBCode {
    float r;
    float g;
    float b;
    float alpha = 0.0f;

   public:
    RGBCode();
    RGBCode(float r, float g, float b);

    void randomRGB();

    float getR();
    float getG();
    float getB();
};
