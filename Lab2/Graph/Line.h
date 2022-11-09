#pragma once

#include "BasicGraph.h"
#include "Point.h"
#include "RGBCode.h"
#include "Util.h"

class Line : public BasicGraph {
    enum LineBase { LINE_BASE_X, LINE_BASE_Y };

    Point start_p;
    Point end_p;

   public:
    // start == end will cause error
    Line();
    Line(Point start_p, Point end_p);
    Line(Point start_p, Point end_p, RGBCode rgbc, float size = DEFAULT_SIZE);

    Point getStartPoint();
    Point getEndPoint();
    Point calXCrossPoint(float x);
    Point calYCrossPoint(float y);

    bool isOnLine(Point p);

    virtual void draw() override;
};
