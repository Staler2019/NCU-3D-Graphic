#pragma once

#include <cmath>  // sqrt, pow

#include "GRGB.h"
#include "Shape/Point.h"
#include "Shape/Shape.h"

class Circle : public Shape {
    Point o;
    float r;

   public:
    inline Circle() : Shape(ShapeType::SHAPE_CIRCLE) {}
    inline Circle(const Point& o, const Point& count_r, const GRGB& rgb, const float edge_size = 1.0f)
        : o(o), Shape(ShapeType::SHAPE_CIRCLE, rgb, edge_size)
    {
        this->r = float(std::sqrt(std::pow(count_r.getX() - o.getX(), 2) + std::pow(count_r.getY() - o.getY(), 2)));
    }

    // void fill();

    virtual void draw() const override;
};
