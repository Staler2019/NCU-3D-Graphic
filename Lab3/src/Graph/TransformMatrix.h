#pragma once

#include "./Point.h"

class TransformMatrix {
    /*
        ref:
       https://www.796t.com/content/1546619583.html?fbclid=IwAR1HlZK3kmqdCC5nccQLMjTVgb5W8pRnO7Ur0GV6pRHr6THiD70Bw7KYbL0
        |m00 m01 m02|
        |m10 m11 m12|
        |m20 m21 m22|
    */
    float m[3][3] = {};

    void matMul(float m_that[3][3]);

   public:
    TransformMatrix();

    void reset();
    void scale(float x_way, float y_way, bool printM = 1);
    void rotate(float degree, bool printM = 1);
    void translate(float del_x, float del_y, bool printM = 1);
    void printMat();

    Point getResult(Point);
};
