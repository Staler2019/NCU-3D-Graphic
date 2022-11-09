#include "Line.h"

#include <cstdlib>  // abs
#include <algorithm> // sort

#include "BasicGraph.h"
#include "Point.h"
#include "RGBCode.h"

Line::Line() : BasicGraph(GraphType::GRAPH_LINE) { return; }

Line::Line(Point start_p, Point end_p) : start_p(start_p), end_p(end_p), BasicGraph(GraphType::GRAPH_LINE) { return; }

Line::Line(Point start_p, Point end_p, RGBCode rgbc, float size)
    : start_p(start_p), end_p(end_p), BasicGraph(GraphType::GRAPH_LINE, rgbc, size)
{
    return;
}

Point Line::getStartPoint() { return this->start_p; }

Point Line::getEndPoint() { return this->end_p; }

void Line::draw()
{
    float base[2];
    float in_counter[2];
    float m;
    enum LineBase lb;

    float (Point::*base_func)(void) = 0;
    float (Point::*in_counter_func)(void) = 0;

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

    base[end_first ^ 1] = (this->end_p.*base_func)();
    in_counter[end_first ^ 1] = (this->end_p.*in_counter_func)();
    base[end_first] = (this->start_p.*base_func)();
    in_counter[end_first] = (this->start_p.*in_counter_func)();

    // cal points
    float b = base[0];
    float ic = in_counter[0];

    do {
        if (lb == LineBase::LINE_BASE_Y)
            Point(ic, b, this->rgbc, this->edge_size).draw();
        else  // lb == LineBase::LINE_BASE_X
            Point(b, ic, this->rgbc, this->edge_size).draw();
        ic += m;
        b += 1.0f;
    } while (b <= base[1]);

    return;
}

Point Line::calXCrossPoint(float x)
{
    // x the same won't happen here
    // y = mx+b
    float m = (this->start_p.getY() - this->end_p.getY()) / (this->start_p.getX() - this->end_p.getX());
    float delta_x = x - this->end_p.getX();
    float delta_y = m * delta_x;

    return Point(x, delta_y + this->end_p.getY());
}

Point Line::calYCrossPoint(float y)
{
    // x = my+b
    float m = (this->start_p.getX() - this->end_p.getX()) / (this->start_p.getY() - this->end_p.getY());
    float delta_y = y - this->end_p.getY();
    float delta_x = m * delta_y;

    return Point(delta_x + this->end_p.getX(), y);
}

bool Line::isOnLine(Point p)
{
    float tmp_x[2] = {this->start_p.getX(), this->end_p.getX()};
    float tmp_y[2] = {this->start_p.getY(), this->end_p.getY()};
    std::sort(tmp_x, tmp_x + 2);
    std::sort(tmp_y, tmp_y + 2);

    float x = p.getX();
    float y = p.getY();

    return (x >= tmp_x[0] && x <= tmp_x[1] && y >= tmp_y[0] && y <= tmp_y[1]);
}
