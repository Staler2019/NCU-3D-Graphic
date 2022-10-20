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

    if (std::abs(this->end_p.getY() - this->start_p.getY()) >
        std::abs(this->end_p.getX() - this->start_p.getX())) {
        lb = LINE_BASE_Y;
        m = (float)(this->end_p.getX() - this->start_p.getX()) /
            (float)(this->end_p.getY() - this->start_p.getY());

        if (this->start_p.getY() > this->end_p.getY()) {
            base[0] = this->end_p.getY();
            in_counter[0] = this->end_p.getX();
            base[1] = this->start_p.getY();
            in_counter[1] = this->start_p.getX();
        } else {
            base[0] = this->start_p.getY();
            in_counter[0] = this->start_p.getX();
            base[1] = this->end_p.getY();
            in_counter[1] = this->end_p.getX();
        }
    } else {
        lb = LINE_BASE_X;
        m = (float)(this->end_p.getY() - this->start_p.getY()) /
            (float)(this->end_p.getX() - this->start_p.getX());

        if (this->start_p.getX() > this->end_p.getX()) {
            base[0] = this->end_p.getX();
            in_counter[0] = this->end_p.getY();
            base[1] = this->start_p.getX();
            in_counter[1] = this->start_p.getY();
        } else {
            base[0] = this->start_p.getX();
            in_counter[0] = this->start_p.getY();
            base[1] = this->end_p.getX();
            in_counter[1] = this->end_p.getY();
        }
    }

    // cal points
    int b = base[0];
    float ic = in_counter[0];

    // test msg
    // std::cout << "draw_line: (" << start_p.getX() << ", " << start_p.getY()
    //           << ")-(" << end_p.getX() << ", " << end_p.getY() << ")"
    //           << std::endl;
    // if (lb == LINE_BASE_Y)
    //     std::cout << "line: (" << ic << ", " << b << ")";
    // else if (lb == LINE_BASE_X)
    //     std::cout << "line: (" << b << ", " << ic << ")";

    do {
        if (lb == LINE_BASE_Y)
            Point(int(std::floor(ic)), b++, this->edge_rgbc, this->edge_size)
                .draw();
        else if (lb == LINE_BASE_X)
            Point(b++, int(std::floor(ic)), this->edge_rgbc, this->edge_size)
                .draw();
        ic += m;
    } while (b <= base[1]);

    // test msg
    // if (lb == LINE_BASE_Y)
    //     std::cout << "-(" << ic-m << ", " << b-1 << ")" << std::endl;
    // else if (lb == LINE_BASE_X)
    //     std::cout << "-(" << b-1 << ", " << ic-m << ")" << std::endl;

    return;
}
