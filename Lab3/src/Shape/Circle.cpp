#include "Shape/Circle.h"

#include <cmath>  // sqrt, pow

#include "Shape/Point.h"
#include "Shape/Shape.h"

void Circle::draw() const
{
    float cutting_point = this->r * float(std::sqrt(2));

    // clockwise with mirror*4
    for (float cp = -cutting_point; cp <= cutting_point; cp++) {
        float before_0 = std::sqrt((cp + r) * (-cp + r));
        // x: y-base: 7/4 pi~ 3/4 pi
        Point(before_0 + this->o.getX(), cp + this->o.getY(), this->rgb, this->edge_size).draw();
        Point(-before_0 + this->o.getX(), cp + this->o.getY(), this->rgb, this->edge_size).draw();
        // y: x-base: 3/4 pi~ 1/4 pi
        Point(cp + this->o.getX(), before_0 + this->o.getY(), this->rgb, this->edge_size).draw();
        Point(cp + this->o.getX(), -before_0 + this->o.getY(), this->rgb, this->edge_size).draw();
    }
}
