#pragma once

#include <iostream>

#include "GRGB.h"
#include "Math/Vector.h"
#include "Shape/Shape.h"

class Point : public Shape {
    float x;
    float y;

   public:
    inline Point() : Shape(ShapeType::SHAPE_POINT) {}
    inline Point(const float x, const float y)
        : x(x), y(y), Shape(ShapeType::SHAPE_POINT)
    {
    }
    inline Point(const Vector2 vec2)
        : x(vec2.v1), y(vec2.v2), Shape(ShapeType::SHAPE_POINT)
    {
    }
    inline Point(const float x, const float y, const GRGB& rgb,
                 const float size = 1.0f)
        : x(x), y(y), Shape(ShapeType::SHAPE_POINT, rgb, size)
    {
    }
    inline Point(const Vector2 vec2, const GRGB& rgb, const float size = 1.0f)
        : x(vec2.v1), y(vec2.v2), Shape(ShapeType::SHAPE_POINT, rgb, size)
    {
    }

    inline float getX() const { return this->x; }
    inline float getY() const { return this->y; }

    inline void setXY(const float x, const float y)
    {
        this->x = x, this->y = y;
    }

    friend std::ostream& operator<<(std::ostream& os, const Point& that);

    virtual void draw() const override;

    inline Vector2 toVec2() const { return Vector2(this->x, this->y); }
};

inline bool isSamePoint(const Point& p1, const Point& p2)
{
    return (p1.getX() == p2.getX() && p1.getY() == p2.getY());
}
