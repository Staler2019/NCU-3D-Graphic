#include "./Line.h"

#include <cmath>    // floor
#include <cstdlib>  // abs

#include "./BasicGraph.h"
#include "./Point.h"
#include "./RGBCode.h"

Line::Line() : BasicGraph() { return; }

Line::Line(Point start_p, Point end_p, RGBCode rgbc, float size)
    : start_p(start_p), end_p(end_p), BasicGraph(rgbc, size) {
    return;
}

void Line::draw() {
    int base[2];
    float in_counter[2];
    float m;
    enum LineBase lb;

    int (Point::*base_func)(void) = 0;
    int (Point::*in_counter_func)(void) = 0;

    if (std::abs(this->end_p.getY() - this->start_p.getY()) >
        std::abs(this->end_p.getX() - this->start_p.getX())) {
        lb = LINE_BASE_Y;
        base_func = &Point::getY;
        in_counter_func = &Point::getX;
    } else {  // LINE_BASE_X
        lb = LINE_BASE_X;
        base_func = &Point::getX;
        in_counter_func = &Point::getY;
    }

    m = (float)((this->end_p.*in_counter_func)() -
                (this->start_p.*in_counter_func)()) /
        (float)((this->end_p.*base_func)() - (this->start_p.*base_func)());

    bool end_first =
        ((this->start_p.*base_func)() > (this->end_p.*base_func)());

    base[end_first ^ 1] = (this->end_p.*base_func)();
    in_counter[end_first ^ 1] = float((this->end_p.*in_counter_func)());
    base[end_first] = (this->start_p.*base_func)();
    in_counter[end_first] = float((this->start_p.*in_counter_func)());

    // cal points
    int b = base[0];
    float ic = in_counter[0];

    do {
        if (lb == LINE_BASE_Y)
            Point(int(std::floor(ic)), b++, this->edge_rgbc, this->edge_size)
                .draw();
        else  // lb == LINE_BASE_X
            Point(b++, int(std::floor(ic)), this->edge_rgbc, this->edge_size)
                .draw();
        ic += m;
    } while (b <= base[1]);

    return;
}
