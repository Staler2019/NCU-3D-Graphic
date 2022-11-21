#pragma once

#include <vector>

#include "BasicGraph.h"
#include "Point.h"
#include "RGBCode.h"
#include "TransformMatrix.h"

class Poly : public BasicGraph {
    std::vector<Point> points;
    bool point_adding_lock = false;  // true: adding point is not allowing

   public:
    Poly();
    Poly(Point first_p, RGBCode rgbc, float edge_size = 1.0f);
    Poly(std::vector<Point> points, RGBCode rgbc, float edge_size = 1.0f);

    void addPoint(Point p, bool toDraw = 1);
    void end(bool toDraw = 1);
    void addTransform(TransformMatrix &tr_mat);

    Point delLastPoint();

    bool isEnded();

    std::size_t size();

    std::vector<Point> getPoints();

    virtual void draw() override;
    void fill();
};
