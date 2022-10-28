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

    // clockwise with mirror*4
    for (float cp = -cutting_point; cp <= cutting_point; cp++) {
        float before_0 = std::sqrt((cp + r) * (-cp + r));
        // x: y-base: 7/4 pi~ 3/4 pi
        Point(int(std::floor(before_0 + this->o.getX())),
              int(std::floor(cp + this->o.getY())), this->edge_rgbc,
              this->edge_size)
            .draw();
        Point(int(std::floor(-before_0 + this->o.getX())),
              int(std::floor(cp + this->o.getY())), this->edge_rgbc,
              this->edge_size)
            .draw();
        // y: x-base: 3/4 pi~ 1/4 pi
        Point(int(std::floor(cp + this->o.getX())),
              int(std::floor(before_0 + this->o.getY())), this->edge_rgbc,
              this->edge_size)
            .draw();
        Point(int(std::floor(cp + this->o.getX())),
              int(std::floor(-before_0 + this->o.getY())), this->edge_rgbc,
              this->edge_size)
            .draw();
    }
    return;
}
