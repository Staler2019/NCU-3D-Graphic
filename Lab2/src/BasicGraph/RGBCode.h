#ifndef __RGBCODE_H__
#define __RGBCODE_H__

class RGBCode {
    float r;
    float g;
    float b;
    float alpha = 0.0f;

   public:
    RGBCode();
    RGBCode(float r, float g, float b);
    void randomRGB();
    inline float getR() { return this->r; }
    inline float getG() { return this->g; }
    inline float getB() { return this->b; }
};

#endif  // __RGBCODE_H__
