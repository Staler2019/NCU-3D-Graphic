#pragma once

#include "GRGB.h"
#include "Shape/Shape.h"

class Point : public Shape {
    float x;
    float y;

   public:
    inline Point() : Shape(ShapeType::SHAPE_POINT) {}
    inline Point(const float x, const float y) : x(x), y(y), Shape(ShapeType::SHAPE_POINT) {}
    inline Point(const float x, const float y, const GRGB& rgb, const float size = 1.0f)
        : x(x), y(y), Shape(ShapeType::SHAPE_POINT, rgb, size)
    {
    }

    inline float getX() const { return this->x; }
    inline float getY() const { return this->y; }

    inline void setXY(const float x, const float y) { this->x = x, this->y = y; }

    virtual void draw() const override;
};

inline bool isSamePoint(const Point& p1, const Point& p2) { return (p1.getX() == p2.getX() && p1.getY() == p2.getY()); }
