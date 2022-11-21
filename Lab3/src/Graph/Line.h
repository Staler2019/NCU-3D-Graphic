#pragma once

#include "BasicGraph.h"
#include "Point.h"
#include "RGBCode.h"

class Line : public BasicGraph {
    enum LineBase { LINE_BASE_X, LINE_BASE_Y };

    Point start_p;
    Point end_p;

   public:
    // start == end will cause error
    Line();
    Line(Point start_p, Point end_p);
    Line(Point start_p, Point end_p, RGBCode rgbc, float size = 1.0f);

    Point getStartPoint();
    Point getEndPoint();
    Point calXCrossPoint(float x);
    Point calYCrossPoint(float y);

    bool isOnLine(Point p);

    virtual void draw() override;
};
