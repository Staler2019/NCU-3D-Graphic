#include "./Circle.h"

#include <cmath>  // sqrt, pow, floor

#include "./BasicGraph.h"
#include "./Point.h"
#include "./RGBCode.h"

Circle::Circle() : BasicGraph() { return; }

Circle::Circle(Point o, Point count_r, RGBCode edge_rgbc, float edge_size)
    : o(o), BasicGraph(edge_rgbc, edge_size) {
    this->r = float(std::sqrt(std::pow(count_r.getX() - o.getX(), 2) +
                        std::pow(count_r.getY() - o.getY(), 2)));

    return;
}

void Circle::draw() {
    float cutting_point = this->r * float(std::sqrt(2));

    // clockwise with mirror
    // y-base: 7/4 pi~ 3/4 pi
    for (float y = -cutting_point; y <= cutting_point; y++) {
        float x_before_x0 = std::sqrt((y + r) * (-y + r));
        Point(int(std::floor(x_before_x0 + this->o.getX())),
              int(std::floor(y + this->o.getY())), this->edge_rgbc,
              this->edge_size)
            .draw();
        Point(int(std::floor(-x_before_x0 + this->o.getX())),
              int(std::floor(y + this->o.getY())), this->edge_rgbc,
              this->edge_size)
            .draw();
    }

    // x-base: 3/4 pi~ 1/4 pi
    for (float x = -cutting_point; x <= cutting_point; x++) {
        float y_before_y0 = std::sqrt((x + r) * (-x + r));
        Point(int(std::floor(x + this->o.getX())),
              int(std::floor(y_before_y0 + this->o.getY())), this->edge_rgbc,
              this->edge_size)
            .draw();
        Point(int(std::floor(x + this->o.getX())),
              int(std::floor(-y_before_y0 + this->o.getY())), this->edge_rgbc,
              this->edge_size)
            .draw();
    }

    return;
}
