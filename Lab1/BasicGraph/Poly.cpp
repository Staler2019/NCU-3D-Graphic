#include "./Poly.h"

#include <vector>

#include "./BasicGraph.h"
#include "./Line.h"
#include "./Point.h"
#include "./RGBCode.h"

Poly::Poly() : BasicGraph() { return; }

Poly::Poly(Point first_p, RGBCode edge_rgbc, float edge_size)
    : BasicGraph(edge_rgbc, edge_size) {
    this->points.emplace_back(first_p);
    return;
}

// Poly::Poly(std::vector<Point> points) : points(points) {
//     this->point_adding_lock = true;
//     return;
// }

void Poly::addPoint(Point p) {
    if (!point_adding_lock && this->points.size() >= 1) {
        Line(this->points.back(), p, this->edge_rgbc, this->edge_size).draw();
        this->points.emplace_back(p);
    }

    return;
}

void Poly::end() {
    Line(this->points.back(), *(this->points.begin()), this->edge_rgbc,
         this->edge_size)
        .draw();
    this->point_adding_lock = true;

    return;
}
