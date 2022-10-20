#ifndef __CIRCLE_H__
#define __CIRCLE_H__

#include "./BasicGraph.h"
#include "./Point.h"
#include "./RGBCode.h"

class Circle : public BasicGraph {
    Point o;
    float r;

   public:
    Circle();
    Circle(Point o, Point count_r, RGBCode edge_rgbc, float edge_size);
    void draw();
    // void setFillRGBC(RGBCode rgbc);
    // void fill();
};

#endif  // __CIRCLE_H__
