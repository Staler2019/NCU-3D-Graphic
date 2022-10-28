#ifndef __POINT_H__
#define __POINT_H__

#include "./BasicGraph.h"
#include "./RGBCode.h"
#include "./Util.h"

class Point : public BasicGraph {
    int x;
    int y;

   public:
    Point();
    Point(int x, int y);
    Point(int x, int y, RGBCode rgbc, float size);
    inline void setXY(int x, int y) {
        this->x = x, this->y = y;
        return;
    }
    inline int getX() { return this->x; }
    inline int getY() { return this->y; }
    void draw();
};

#endif  // __POINT_H__
