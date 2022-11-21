#pragma once

#include "BasicGraph.h"
#include "RGBCode.h"
#include "Util.h"

class Point : public BasicGraph {
    float x;
    float y;

   public:
    Point();
    Point(float x, float y);
    Point(float x, float y, RGBCode rgbc, float size = 1.0f);

    float getX();
    float getY();

    void setXY(float x, float y);

    virtual void draw() override;
};

bool isSamePoint(Point p1, Point p2);
