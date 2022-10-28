#ifndef __POLY_H__
#define __POLY_H__

#include <vector>

#include "./BasicGraph.h"
#include "./Point.h"
#include "./RGBCode.h"

class Poly : public BasicGraph {
    std::vector<Point> points;
    bool point_adding_lock = false;  // true: adding point is not allowing
    RGBCode rgb;

   public:
    Poly();
    Poly(Point first_p, RGBCode edge_rgbc, float edge_size);
    // Poly(std::vector<Point> points, RGBCode edge_rgbc, float edge_size);
    void addPoint(Point p);
    void end();
    inline bool isEnded() { return this->point_adding_lock; }
    Point delLastPoint();
    inline std::size_t size() { return this->points.size(); }
};

#endif  // __POLY_H__
