#include "Shape/Line.h"

#include <algorithm>  // sort
#include <cstdlib>    // abs
#include <iostream>

#include "GRGB.h"
#include "Shape/Point.h"
#include "Shape/Shape.h"

void Line::draw() const
{
    if (isSamePoint(this->start_p, this->end_p)) {
        Point(this->start_p.getX(), this->start_p.getY(), this->rgb, this->edge_size).draw();
        return;
    }

    float base[2];
    float in_counter[2];
    float m;
    enum LineBase lb;

    float (Point::*base_func)(void) const = 0;
    float (Point::*in_counter_func)(void) const = 0;

    if (std::abs(this->end_p.getY() - this->start_p.getY()) > std::abs(this->end_p.getX() - this->start_p.getX())) {
        lb = LineBase::LINE_BASE_Y;
        base_func = &Point::getY;
        in_counter_func = &Point::getX;
    }
    else {
        lb = LineBase::LINE_BASE_X;
        base_func = &Point::getX;
        in_counter_func = &Point::getY;
    }

    m = ((this->end_p.*in_counter_func)() - (this->start_p.*in_counter_func)()) /
        ((this->end_p.*base_func)() - (this->start_p.*base_func)());

    bool end_first = ((this->start_p.*base_func)() > (this->end_p.*base_func)());

    base[!end_first] = (this->end_p.*base_func)();
    in_counter[!end_first] = (this->end_p.*in_counter_func)();
    base[end_first] = (this->start_p.*base_func)();
    in_counter[end_first] = (this->start_p.*in_counter_func)();

    // cal points
    float b = base[0];
    float ic = in_counter[0];

    do {
        if (lb == LineBase::LINE_BASE_Y)
            Point(ic, b, this->rgb, this->edge_size).draw();
        else  // lb == LineBase::LINE_BASE_X
            Point(b, ic, this->rgb, this->edge_size).draw();
        ic += m;
        b += 1.0f;
    } while (b <= base[1]);
}
