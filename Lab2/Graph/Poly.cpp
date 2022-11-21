#include "Poly.h"

#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

#include "BasicGraph.h"
#include "ExtendFunction.h"
#include "Line.h"
#include "Point.h"
#include "RGBCode.h"
#include "TransformMatrix.h"
#include "Util.h"

Poly::Poly() : BasicGraph(GraphType::GRAPH_POLY) { return; }

Poly::Poly(Point first_p, RGBCode rgbc, float edge_size) : BasicGraph(GraphType::GRAPH_POLY, rgbc, edge_size)
{
    this->points.emplace_back(first_p);
    return;
}

Poly::Poly(std::vector<Point> points, RGBCode rgbc, float edge_size)
    : points(points), BasicGraph(GraphType::GRAPH_POLY, rgbc, edge_size)
{
    this->point_adding_lock = true;
    return;
}

void Poly::addPoint(Point p, bool toDraw /* = 1*/)
{
    if (!point_adding_lock && this->points.size() >= 1) {
        if (toDraw) Line(this->points.back(), p, this->rgbc, this->edge_size).draw();
        this->points.emplace_back(p);
    }
    return;
}

void Poly::end(bool toDraw /* = 1*/)
{
    if (toDraw) Line(this->points.back(), *(this->points.begin()), this->rgbc, this->edge_size).draw();
    this->point_adding_lock = true;
    return;
}

Point Poly::delLastPoint()
{
    if (this->points.size() == 0) {
        std::cerr << "Poly runtime error because there's no element is poly's "
                     "vector\n";
        exit(0);
    }

    if (this->point_adding_lock) {
        this->point_adding_lock = 0;
        return *(this->points.begin());
    }

    Point tmp = this->points.back();
    this->points.pop_back();
    return tmp;
}

bool Poly::isEnded() { return this->point_adding_lock; }

std::size_t Poly::size() { return this->points.size(); }

void Poly::draw()
{
    if (!this->point_adding_lock) {
        std::cerr << "Please true the Poly adding lock before drawing\n";
        return;
    }
    else if (this->points.size() <= 1) {
        std::cerr << "Please false the Poly adding lock and add some points\n";
        return;
    }

    // std::cerr << "The poly drawing " << this->points.size() << " points: ";

    std::vector<Point>::iterator pit1 = this->points.begin();
    std::vector<Point>::iterator pit2 = this->points.begin();
    pit2++;

    // std::cerr << "(" << pit1->getX() << ":" << pit1->getY() << "), ";

    while (pit2 != this->points.end()) {
        // std::cerr << "(" << pit2->getX() << ":" << pit2->getY() << "), ";
        Line(*pit1, *pit2, this->rgbc, this->edge_size).draw();
        pit1++, pit2++;
    }

    if (pit2 == this->points.end())
        Line(this->points.back(), *(this->points.begin()), this->rgbc, this->edge_size).draw();

    // std::cerr << std::endl;

    return;
}

void Poly::addTransform(TransformMatrix &tr_mat)
{
    for (Point &p : this->points) p = tr_mat.getResult(p);
    return;
}

std::vector<Point> Poly::getPoints() { return this->points; }

void Poly::fill()
{
    // test
    std::cerr << "Now filling the poly: ";
    for (auto &p : this->points) std::cerr << "(" << p.getX() << ", " << p.getY() << "), ";
    std::cerr << std::endl;
    // !test

    // 1. error handle
    switch (this->size()) {
        case 0: {  // error
            std::cerr << "Poly fill points are none\n";
            return;
        }
        case 1: {  // Point
            Point(this->points[0].getX(), this->points[0].getY(), this->rgbc, 1.0f).draw();
            return;
        }
        case 2: {  // Line
            Line(this->points[0], this->points[1], this->rgbc, 1.0f).draw();
            return;
        }
    }

    // 2. get y's highest & lowest
    float highest_y = this->points[0].getY();
    float lowest_y = this->points[0].getY();

    for (int i = 1; i < this->points.size(); i++) {
        float now_y = this->points[i].getY();

        if (now_y > highest_y)
            highest_y = now_y;
        else if (now_y < lowest_y)  // highest != lowest
            lowest_y = now_y;
    }

    // 3. store all lines
    std::vector<std::pair<Line, bool>> lines;  // line, bool(line_top_used)
    Point *last_point = &this->points[this->points.size() - 1];

    for (int i = 0; i < this->points.size(); i++) {
        Point *now_point = &this->points[i];

        lines.emplace_back(Line(*last_point, *now_point), 0);

        last_point = now_point;
    }

    // 4. fill graph on thw window

    // for each y
    for (float i = highest_y; i >= lowest_y; i -= 1) {
        // (1) for each y check its edge points
        std::vector<std::pair<Point, bool>> line_edge_points;  // point, first_top(0)/others(1);

        for (auto &line : lines) {
            Line &l = line.first;
            bool &line_top_used = line.second;

            // for line is not parallel to x-axis
            if (l.getStartPoint().getY() != l.getEndPoint().getY()) {
                // a. calculate cross point
                Point cal_point = l.calYCrossPoint(i);

                // b. error checking
                if (cal_point.getY() != i) std::cerr << "CalY error in fill\n";

                // c. check calculated point is on line
                if (l.isOnLine(cal_point)) {
                    if (line_top_used)
                        line_edge_points.emplace_back(cal_point, 1);
                    else {
                        line_edge_points.emplace_back(cal_point, 0);
                        line_top_used = 1;
                    }
                }
            }
        }

        // (2) sort by x
        std::sort(
            line_edge_points.begin(), line_edge_points.end(),
            [](std::pair<Point, bool> p1, std::pair<Point, bool> p2) { return p1.first.getX() < p2.first.getX(); });

        // (3) draw line
        // (3-1) case give away
        if (line_edge_points.size() == 0) {
            // std::cerr << "No points on y=" << i << "\n";
            continue;
        }
        else if (line_edge_points.size() == 1) {
            Point(line_edge_points[0].first.getX(), line_edge_points[0].first.getY(), this->rgbc, 1.0f).draw();
            std::cerr << "\tdraw point: (" << line_edge_points[0].first.getX() << ", "
                      << line_edge_points[0].first.getY() << ")\n";
            continue;
        }

        // (3-2) other cases
        const Point &tmp_p1 = line_edge_points[0].first;
        const Point &tmp_p2 = line_edge_points[1].first;
        const bool &tmp_first_1 = line_edge_points[0].second;
        const bool &tmp_first_2 = line_edge_points[1].second;

        bool drawState = 1;
        bool last_edge_state =
            (!(tmp_first_1 ^ tmp_first_2) && isSamePoint(tmp_p1, tmp_p2)) ? 0 : 1;  // is top/down vertex
        last_point = nullptr;

        for (int j = 0; j < line_edge_points.size(); j++) {
            Point *now_point = &line_edge_points[j].first;
            const bool now_edge_state = line_edge_points[j].second;

            if (last_point != nullptr) {
                if (last_edge_state ^ now_edge_state) {  // xor
                    if (!isSamePoint(*last_point, *now_point)) {
                        if (drawState) {
                            Line(*last_point, *now_point, this->rgbc, 1.0f).draw();
                            std::cerr << "\tdraw line: (" << last_point->getX() << ", " << last_point->getY() << ")-("
                                      << now_point->getX() << ", " << now_point->getY() << ")\n";
                        }
                        drawState ^= 1;
                    }
                }
                else {
                    if (isSamePoint(*last_point, *now_point)) {
                        Point(last_point->getX(), last_point->getY(), this->rgbc, 1.0f).draw();
                        std::cerr << "\tdraw point: (" << last_point->getX() << ", " << last_point->getY() << ")\n";
                    }
                    else {
                        if (drawState) {
                            Line(*last_point, *now_point, this->rgbc, 1.0f).draw();
                            std::cerr << "\tdraw line: (" << last_point->getX() << ", " << last_point->getY() << ")-("
                                      << now_point->getX() << ", " << now_point->getY() << ")\n";
                        }
                        drawState ^= 1;
                    }
                }
            }

            last_point = now_point;
            last_edge_state = now_edge_state;
        }
    }

    return;
}
