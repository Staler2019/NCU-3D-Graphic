#include "Shape/Line.h"

#include <algorithm>  // sort
#include <cstdlib>    // abs
#include <iostream>
#include <vector>

#include "GMath.h"
#include "GRGB.h"
#include "Shape/Point.h"
#include "Shape/Shape.h"

Line::Line(const Point &start_p, const Point &end_p, const GRGB &rgb,
           const float size /* = 1.0f*/)
    : start_p(start_p), end_p(end_p), Shape(ShapeType::SHAPE_LINE, rgb, size)
{
    if (isSamePoint(start_p, end_p)) {
        std::cerr << "Line constructs with same point\n";
        this->~Line();
    }
}

Line::Line(const Vector2 &start_vec2, const Vector2 &end_vec2, const GRGB &rgb,
           const float size /* = 1.0f*/)
    : start_p(Point(start_vec2)),
      end_p(Point(end_vec2)),
      Shape(ShapeType::SHAPE_LINE, rgb, size)
{
    if (isSamePoint(this->start_p, this->end_p)) {
        std::cerr << "Line constructs with same point\n";
        this->~Line();
    }
}

Point Line::calXCrossPoint(const float x) const
{
    // x the same won't happen here
    // y = mx+b
    float m = (this->start_p.getY() - this->end_p.getY()) /
              (this->start_p.getX() - this->end_p.getX());
    float delta_x = x - this->end_p.getX();
    float delta_y = m * delta_x;

    return Point(x, delta_y + this->end_p.getY());
}

Point Line::calYCrossPoint(const float y) const
{
    // x = my+b
    float m = (this->start_p.getX() - this->end_p.getX()) /
              (this->start_p.getY() - this->end_p.getY());
    float delta_y = y - this->end_p.getY();
    float delta_x = m * delta_y;

    return Point(delta_x + this->end_p.getX(), y);
}

bool Line::isOnLine(const Point &p) const
{
    float tmp_x[2] = {this->start_p.getX(), this->end_p.getX()};
    float tmp_y[2] = {this->start_p.getY(), this->end_p.getY()};

    std::sort(tmp_x, tmp_x + 2);
    std::sort(tmp_y, tmp_y + 2);

    float x = p.getX();
    float y = p.getY();

    return (x >= tmp_x[0] && x <= tmp_x[1] && y >= tmp_y[0] && y <= tmp_y[1]);
}

void Line::draw() const
{
    if (isSamePoint(this->start_p, this->end_p)) {
        Point(this->start_p.getX(), this->start_p.getY(), this->rgb,
              this->edge_size)
            .draw();
        return;
    }

    float base[2];
    float in_counter[2];
    float m;
    enum LineBase lb;

    float (Point::*base_func)(void) const = 0;
    float (Point::*in_counter_func)(void) const = 0;

    if (std::abs(this->end_p.getY() - this->start_p.getY()) >
        std::abs(this->end_p.getX() - this->start_p.getX())) {
        lb = LineBase::LINE_BASE_Y;
        base_func = &Point::getY;
        in_counter_func = &Point::getX;
    }
    else {
        lb = LineBase::LINE_BASE_X;
        base_func = &Point::getX;
        in_counter_func = &Point::getY;
    }

    m = ((this->end_p.*in_counter_func)() -
         (this->start_p.*in_counter_func)()) /
        ((this->end_p.*base_func)() - (this->start_p.*base_func)());

    bool end_first =
        ((this->start_p.*base_func)() > (this->end_p.*base_func)());

    base[!end_first] = (this->end_p.*base_func)();
    in_counter[!end_first] = (this->end_p.*in_counter_func)();
    base[end_first] = (this->start_p.*base_func)();
    in_counter[end_first] = (this->start_p.*in_counter_func)();

    // cal points
    float b = base[0];
    float ic = in_counter[0];

    do {
        if (lb == LineBase::LINE_BASE_Y)
            Point(ic, b, this->rgb, this->edge_size).draw();
        else  // lb == LineBase::LINE_BASE_X
            Point(b, ic, this->rgb, this->edge_size).draw();
        ic += m;
        b += 1.0f;
    } while (b <= base[1]);
}

// float Line::getValue(const Point &p) const
// {
//     std::cerr << "start_p: " << this->start_p << ", end_p: " << this->end_p
//               << "\n";

//     return (p.getX() - this->start_p.getX()) -
//            (this->end_p.getX() - this->start_p.getX()) *
//                (p.getY() - this->start_p.getY()) /
//                (this->end_p.getY() - this->start_p.getY());

//     // Matrix2 mat2inv;
//     // Matrix2 mat;
//     // {
//     //     float tmp_m[2][2];

//     //     tmp_m[0][0] = this->start_p.getX();
//     //     tmp_m[0][1] = 1;
//     //     tmp_m[1][0] = this->end_p.getX();
//     //     tmp_m[1][1] = 1;

//     //     mat2inv = Matrix2(tmp_m).inverse();
//     //     mat = Matrix2(tmp_m);
//     // }

//     // if (!mat2inv.isExist()) {
//     //     std::cerr << "mat2inv error in Line::getValue\n";
//     //     std::cerr << "mat:\n";
//     //     for (int i = 0; i < 2; i++) {
//     //         std::cerr << "| ";
//     //         for (int j = 0; j < 2; j++) std::cerr << mat.m[i][j] << " ";
//     //         std::cerr << "|\n";
//     //     }
//     //     exit(1);
//     // }

//     // // ax+b = y
//     // Vector2 ab = mat2inv * Vector2(this->start_p.getY(), this->end_p.getY());

//     // // ax+b-y
//     // return (ab.v1 * p.getX() + ab.v2 - p.getY())*-1.0f;
// }

std::vector<Vector3> Line3D::getLinePoints()
    const  // TODO. Vector3::v3 cannot use to compare
{
    std::vector<Vector3> line_points;

    // error handle
    if (this->start.hasSameXY(this->end)) {
        line_points.push_back(this->start);
        return line_points;
    }

    const Vector3 del_vec = this->end - this->start;
    float biggest_axis = abs(del_vec.v1);
    // init
    // enum LineBase lb = LineBase::LINE_BASE_X;
    float Vector3::*base = &Vector3::v1;
    Vector3 (Line3D::*base_func)(const float) const = &Line3D::calcXCrossVec;

    if (biggest_axis < abs(del_vec.v2)) {
        biggest_axis = abs(del_vec.v2);
        // lb = LineBase::LINE_BASE_Y;
        base = &Vector3::v2;
        base_func = &Line3D::calcYCrossVec;
    }

    if (base == nullptr || base_func == nullptr) {
        std::cerr << "Error in setting function/member pointers "
                     "(Line3D::getLinePoints)\n";
        exit(1);
    }

    const Vector3 *vec_start = nullptr;
    const Vector3 *vec_end = nullptr;

    if (del_vec.*base > 0) {
        vec_start = &this->start;
        vec_end = &this->end;
    }
    else if (del_vec.*base < 0) {
        vec_start = &this->end;
        vec_end = &this->start;
        // del_vec *= -1;
    }
    else if (del_vec.*base == 0) {
        std::cerr << "Error in del_vec.*base == 0"
                     "(Line3D::getLinePoints)\n";
        std::cerr << "On Line3D: " << this->start << "-" << this->end << "\n";
        exit(1);
    }

    const float end_base = vec_end->*base;
    float start_base = vec_start->*base;

    do {
        line_points.emplace_back((this->*base_func)(start_base));
        start_base++;
    } while (start_base <= end_base);

    return line_points;
}

Vector3 Line3D::calcZCrossVec(const float z) const
{
    if (this->start.v3 == this->end.v3) {
        std::cerr << "Don't put same point in Line3D in "
                     "calcV3CrossVec: "
                  << this->start << std::endl;
        exit(1);
    }

    Vector3 m = this->start - this->end;
    float scale = (z - this->end.v3) / m.v3;
    return this->end + m * scale;
}

Vector3 Line3D::calcXCrossVec(const float x) const
{
    if (this->start.v1 == this->end.v1) {
        std::cerr << "Don't put same point in Line3D in "
                     "calcXCrossVec: "
                  << this->start << std::endl;
        // return this->start;
        exit(1);
    }

    // std::cerr << "Line3D: " << this->start << "-" << this->end << "\n";
    // x = my+b
    Vector3 del_vec = this->end - this->start;
    float scale = (x - this->start.v1) / del_vec.v1;
    // std::cerr << "when y = " << y << ", value of return vec3 is "
    //           << this->start + del_vec * scale << "\n";

    return this->start + del_vec * scale;
}

Vector3 Line3D::calcYCrossVec(const float y) const
{
    if (this->start.v2 == this->end.v2) {
        std::cerr << "Don't put same point in Line3D in "
                     "calcYCrossVec: "
                  << this->start << std::endl;
        // return this->start;
        exit(1);
    }

    // std::cerr << "Line3D: " << this->start << "-" << this->end << "\n";
    // x = my+b
    Vector3 del_vec = this->end - this->start;
    float scale = (y - this->start.v2) / del_vec.v2;
    // std::cerr << "when y = " << y << ", value of return vec3 is "
    //           << this->start + del_vec * scale << "\n";

    return this->start + del_vec * scale;
}

bool Line3D::isOnLine(const Vector3 &vec) const
{
    float tmp_x[2] = {this->start.v1, this->end.v1};
    float tmp_y[2] = {this->start.v2, this->end.v2};
    float tmp_z[2] = {this->start.v3, this->end.v3};

    std::sort(tmp_x, tmp_x + 2);
    std::sort(tmp_y, tmp_y + 2);
    std::sort(tmp_z, tmp_z + 2);

    const float &x = vec.v1;
    const float &y = vec.v2;
    const float &z = vec.v3;

    return (x >= tmp_x[0] && x <= tmp_x[1] && y >= tmp_y[0] && y <= tmp_y[1] &&
            z >= tmp_z[0] && z <= tmp_z[1]);
}