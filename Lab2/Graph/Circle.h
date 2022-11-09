#pragma once

#include "BasicGraph.h"
#include "Point.h"
#include "RGBCode.h"
#include "Util.h"

class Circle : public BasicGraph {
    Point o;
    float r;

   public:
    Circle();
    Circle(Point o, Point count_r, RGBCode rgbc, float edge_size = DEFAULT_SIZE);

    // void fill();

    virtual void draw() override;
};
