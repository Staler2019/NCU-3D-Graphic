#include "Circle.h"

#include <cmath>  // sqrt, pow

#include "BasicGraph.h"
#include "Point.h"
#include "RGBCode.h"

Circle::Circle() : BasicGraph(GraphType::GRAPH_CIRCLE) { return; }

Circle::Circle(Point o, Point count_r, RGBCode rgbc, float edge_size)
    : o(o), BasicGraph(GraphType::GRAPH_CIRCLE, rgbc, edge_size)
{
    this->r = float(std::sqrt(std::pow(count_r.getX() - o.getX(), 2) + std::pow(count_r.getY() - o.getY(), 2)));
    return;
}

void Circle::draw()
{
    float cutting_point = this->r * float(std::sqrt(2));

    // clockwise with mirror*4
    for (float cp = -cutting_point; cp <= cutting_point; cp++) {
        float before_0 = std::sqrt((cp + r) * (-cp + r));
        // x: y-base: 7/4 pi~ 3/4 pi
        Point(before_0 + this->o.getX(), cp + this->o.getY(), this->rgbc, this->edge_size).draw();
        Point(-before_0 + this->o.getX(), cp + this->o.getY(), this->rgbc, this->edge_size).draw();
        // y: x-base: 3/4 pi~ 1/4 pi
        Point(cp + this->o.getX(), before_0 + this->o.getY(), this->rgbc, this->edge_size).draw();
        Point(cp + this->o.getX(), -before_0 + this->o.getY(), this->rgbc, this->edge_size).draw();
    }
    return;
}
