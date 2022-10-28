#ifndef __LINE_H__
#define __LINE_H__

#include "./BasicGraph.h"
#include "./Point.h"
#include "./RGBCode.h"

class Line : public BasicGraph {
    enum LineBase { LINE_BASE_X, LINE_BASE_Y };

    Point start_p;
    Point end_p;

   public:
    // start == end will cause error
    Line();
    Line(Point start_p, Point end_p, RGBCode rgbc, float size);
    // inline Point getStartPoint() { return this->start_p; }
    // inline Point getEndPoint() { return this->end_p; }
    void draw();
};

#endif  // __LINE_H__
