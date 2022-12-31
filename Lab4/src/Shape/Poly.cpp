#include "Shape/Poly.h"

#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

#include "GRGB.h"
#include "Math/Matrix.h"
#include "Math/Vector.h"
#include "Shape/Line.h"
#include "Shape/Point.h"
#include "Shape/Shape.h"
#include "Transform.h"

void Poly::draw() const
{
    if (!this->point_adding_lock) {
        std::cerr << "Please true the Poly adding lock "
                     "before drawing\n";
        return;
    }
    else if (this->points.size() <= 1) {
        std::cerr << "Please false the Poly adding lock "
                     "and add some points\n";
        return;
    }

    // std::cerr << "The poly drawing " <<
    // this->points.size() << " points: ";

    std::vector<Point>::const_iterator pit1 = this->points.begin();
    std::vector<Point>::const_iterator pit2 = this->points.begin();
    pit2++;

    // std::cerr << "(" << pit1->getX() << ":" <<
    // pit1->getY() << "), ";

    while (pit2 != this->points.end()) {
        // std::cerr << "(" << pit2->getX() << ":" <<
        // pit2->getY() << "), ";
        Line(*pit1, *pit2, this->rgb, this->edge_size).draw();
        pit1++, pit2++;
    }

    if (pit2 == this->points.end())
        Line(this->points.back(), *(this->points.begin()), this->rgb,
             this->edge_size)
            .draw();

    // std::cerr << std::endl;
}

void Poly::addTransform(Transform2D &tr)
{
    for (Point &p : this->points) p = tr.getResult(p);
}

void Poly::fill() const
{
    // test
    std::cerr << "Now filling the poly: ";
    for (auto &p : this->points)
        std::cerr << "(" << p.getX() << ", " << p.getY() << "), ";
    std::cerr << std::endl;
    // !test

    // 1. error handle
    switch (this->size()) {
        case 0: {  // error
            std::cerr << "Poly fill points are none\n";
            return;
        }
        case 1: {  // Point
            Point(this->points[0].getX(), this->points[0].getY(), this->rgb,
                  1.0f)
                .draw();
            return;
        }
        case 2: {  // Line
            Line(this->points[0], this->points[1], this->rgb, 1.0f).draw();
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
    const Point *last_point = &this->points[this->points.size() - 1];

    for (int i = 0; i < this->points.size(); i++) {
        const Point *now_point = &this->points[i];

        lines.emplace_back(Line(*last_point, *now_point), 0);

        last_point = now_point;
    }

    // 4. fill graph on thw window

    // for each y
    for (float i = highest_y; i >= lowest_y; i -= 1) {
        // (1) for each y check its edge points
        std::vector<std::pair<Point, bool>>
            line_edge_points;  // point,
                               // first_top(0)/others(1);

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
        std::sort(line_edge_points.begin(), line_edge_points.end(),
                  [](std::pair<Point, bool> p1, std::pair<Point, bool> p2) {
                      return p1.first.getX() < p2.first.getX();
                  });

        // (3) draw line
        // (3-1) case give away
        if (line_edge_points.size() == 0) {
            // std::cerr << "No points on y=" << i << "\n";
            continue;
        }
        else if (line_edge_points.size() == 1) {
            Point(line_edge_points[0].first.getX(),
                  line_edge_points[0].first.getY(), this->rgb, 1.0f)
                .draw();
            std::cerr << "\tdraw point: (" << line_edge_points[0].first.getX()
                      << ", " << line_edge_points[0].first.getY() << ")\n";
            continue;
        }

        // (3-2) other cases
        const Point &tmp_p1 = line_edge_points[0].first;
        const Point &tmp_p2 = line_edge_points[1].first;
        const bool &tmp_first_1 = line_edge_points[0].second;
        const bool &tmp_first_2 = line_edge_points[1].second;

        bool drawState = 1;
        bool last_edge_state =
            (!(tmp_first_1 ^ tmp_first_2) && isSamePoint(tmp_p1, tmp_p2))
                ? 0
                : 1;  // is top/down vertex
        last_point = nullptr;

        for (int j = 0; j < line_edge_points.size(); j++) {
            Point *now_point = &line_edge_points[j].first;
            const bool now_edge_state = line_edge_points[j].second;

            if (last_point != nullptr) {
                if (last_edge_state ^ now_edge_state) {  // xor
                    if (!isSamePoint(*last_point, *now_point)) {
                        if (drawState) {
                            Line(*last_point, *now_point, this->rgb, 1.0f)
                                .draw();
                            std::cerr << "\tdraw line: (" << last_point->getX()
                                      << ", " << last_point->getY() << ")-("
                                      << now_point->getX() << ", "
                                      << now_point->getY() << ")\n";
                        }
                        drawState ^= 1;
                    }
                }
                else {
                    if (isSamePoint(*last_point, *now_point)) {
                        Point(last_point->getX(), last_point->getY(), this->rgb,
                              1.0f)
                            .draw();
                        std::cerr << "\tdraw point: (" << last_point->getX()
                                  << ", " << last_point->getY() << ")\n";
                    }
                    else {
                        if (drawState) {
                            Line(*last_point, *now_point, this->rgb, 1.0f)
                                .draw();
                            std::cerr << "\tdraw line: (" << last_point->getX()
                                      << ", " << last_point->getY() << ")-("
                                      << now_point->getX() << ", "
                                      << now_point->getY() << ")\n";
                        }
                        drawState ^= 1;
                    }
                }
            }

            last_point = now_point;
            last_edge_state = now_edge_state;
        }
    }
}

// std::vector<Vector3> Poly3D::getFillPoints() const
// {
//     std::vector<Vector3> poly_fill_points;

//     // test
//     std::cerr << "Now filling the poly: ";
//     for (auto &p : this->vertexes) std::cerr << p << ",
//     "; std::cerr << std::endl;
//     // !test

//     // 1. error handle
//     switch (this->vertexes.size()) {
//         case 0: {  // error
//             // std::cerr << "Poly fill points are
//             none\n"; return poly_fill_points;
//         }
//         case 1: {  // Point
//             poly_fill_points.push_back(this->vertexes[0]);
//             // std::cerr << "\tdraw point: " <<
//             this->vertexes[0] << "\n"; return
//             poly_fill_points;
//         }
//         case 2: {  // Line
//             for (auto &p : Line3D(this->vertexes[0],
//             this->vertexes[1]).getLinePoints())
//                 poly_fill_points.push_back(p);
//             // std::cerr << "\tdraw line: " <<
//             this->vertexes[0] << "-" << this->vertexes[1]
//             //           << "\n";
//             return poly_fill_points;
//         }
//     }

//     // 2. get y's highest & lowest
//     float highest_y = this->vertexes[0].v2;
//     float lowest_y = this->vertexes[0].v2;

//     for (int i = 1; i < this->vertexes.size(); i++) {
//         const float &now_y = this->vertexes[i].v2;

//         if (now_y > highest_y)
//             highest_y = now_y;
//         else if (now_y < lowest_y)  // highest != lowest
//             lowest_y = now_y;
//     }

//     // highest_y = floor(highest_y);
//     // lowest_y = ceil(lowest_y);

//     // 3. store all lines
//     // line, bool(line_top_used)
//     std::vector<std::pair<Line3D, bool>> lines;
//     const Vector3 *last_point =
//     &this->vertexes[this->vertexes.size() - 1];

//     for (int i = 0; i < this->vertexes.size(); i++) {
//         const Vector3 *now_point = &this->vertexes[i];

//         lines.emplace_back(Line3D(*last_point,
//         *now_point), 0);

//         last_point = now_point;
//     }

//     // 4. fill graph on thw window

//     // for each y
//     for (float i = highest_y; i >= lowest_y; i -= 1) {
//         // (1) for each y check its edge points
//         // point, first_top(0)/others(1);
//         std::vector<std::pair<Vector3, bool>>
//         line_edge_points;

//         for (auto &line : lines) {
//             Line3D &l = line.first;
//             bool &line_top_used = line.second;

//             // for line is not parallel to x-axis //
//             parallel is counted by others lines if
//             (l.getStartVec3().v2 != l.getEndVec3().v2) {
//                 // a. calculate cross point
//                 Vector3 cal_point = l.calcYCrossVec(i);

//                 // b. error checking
//                 if (cal_point.v2 != i) std::cerr << "CalY
//                 error in Poly3D::getFillPoints\n";

//                 // c. check calculated point is on line
//                 if (l.isOnLine(cal_point)) {
//                     if (line_top_used)
//                         line_edge_points.emplace_back(cal_point,
//                         1);
//                     else {
//                         line_edge_points.emplace_back(cal_point,
//                         0); line_top_used = 1;
//                     }
//                 }
//                 else {
//                     // std::cerr << cal_point << " is not
//                     on Line3D: " << l.getStartVec3() <<
//                     "-"
//                     //           << l.getEndVec3() <<
//                     "\n";
//                 }
//             }
//         }

//         // (2) sort by x
//         std::sort(line_edge_points.begin(),
//         line_edge_points.end(),
//                   [](std::pair<Vector3, bool> p1,
//                   std::pair<Vector3, bool> p2) {
//                       return p1.first.v1 < p2.first.v1;
//                   });

//         // (3) draw line
//         // (3-1) case give away
//         if (line_edge_points.size() == 0) {
//             // std::cerr << "No points on y=" << i <<
//             "\n"; continue;
//         }
//         else if (line_edge_points.size() == 1) {
//             poly_fill_points.push_back(line_edge_points[0].first);
//             // std::cerr << "\tdraw point: " <<
//             line_edge_points[0].first << "\n"; continue;
//         }

//         // (3-2) other cases
//         const Vector3 &tmp_p1 =
//         line_edge_points[0].first; const Vector3 &tmp_p2
//         = line_edge_points[1].first; const bool
//         &tmp_first_1 = line_edge_points[0].second; const
//         bool &tmp_first_2 = line_edge_points[1].second;

//         bool drawState = 1;
//         // 1: is top/down vertex
//         bool last_edge_state = (!(tmp_first_1 ^
//         tmp_first_2) && tmp_p1.hasSameXY(tmp_p2)) ? 0 :
//         1; last_point = nullptr;

//         for (int j = 0; j < line_edge_points.size(); j++)
//         {
//             Vector3 *now_point =
//             &line_edge_points[j].first; const bool
//             now_edge_state = line_edge_points[j].second;

//             if (last_point != nullptr) {
//                 if (last_edge_state ^ now_edge_state) {
//                 // xor
//                     if
//                     (!last_point->hasSameXY(*now_point))
//                     {
//                         if (drawState) {
//                             for (auto &p :
//                             Line3D(*last_point,
//                             *now_point).getLinePoints())
//                                 poly_fill_points.push_back(p);
//                             // std::cerr << "\tdraw line:
//                             " << *last_point << "-" <<
//                             *now_point
//                             //           << "\n";
//                         }
//                         drawState ^= 1;
//                     }
//                 }
//                 else {
//                     if
//                     (!last_point->hasSameXY(*now_point))
//                     {
//                         poly_fill_points.push_back(*last_point);
//                         // std::cerr << "\tdraw point: "
//                         << *last_point << "\n";
//                     }
//                     else {
//                         if (drawState) {
//                             for (auto &p :
//                             Line3D(*last_point,
//                             *now_point).getLinePoints())
//                                 poly_fill_points.push_back(p);
//                             // std::cerr << "\tdraw line:
//                             " << *last_point << "-" <<
//                             *now_point
//                             //           << "\n";
//                         }
//                         drawState ^= 1;
//                     }
//                 }
//             }

//             last_point = now_point;
//             last_edge_state = now_edge_state;
//         }
//     }
//     return poly_fill_points;
// }

std::vector<std::vector<Vector3>> Poly3D::toTriangles() const
{
    if (this->vertexes.size() <= 3)
        return std::vector<std::vector<Vector3>>(1, this->vertexes);

    std::vector<std::vector<Vector3>> result;
    int action_count = 0;
    auto it_begin = this->vertexes.begin();
    auto it_end = this->vertexes.end();
    auto it_tmp = it_begin;

    for (it_begin++, it_end--; it_begin != it_end; action_count++) {
        std::vector<Vector3> tmp_vertexes;

        if (action_count % 2 == 0) {
            tmp_vertexes.push_back(*it_tmp);
            tmp_vertexes.push_back(*it_begin);
            tmp_vertexes.push_back(*it_end);

            it_tmp = it_end;
            it_end--;
        }
        else {
            tmp_vertexes.push_back(*it_begin);
            tmp_vertexes.push_back(*it_end);
            tmp_vertexes.push_back(*it_tmp);

            it_tmp = it_begin;
            it_begin++;
        }

        result.emplace_back(tmp_vertexes);
    }

    return result;
}

std::vector<Vector3> getTriangleFillVec3s(const std::vector<Vector3> &tri_vec3s)
{
    // error handle
    switch (tri_vec3s.size()) {
        case 0:
        case 1: {
            return tri_vec3s;
        }

        case 2: {
            return Line3D(tri_vec3s[0], tri_vec3s[1]).getLinePoints();
        }
    }

    if (tri_vec3s.size() > 3) {
        std::cerr
            << "Input triangle vertexes > 3 error in getTriangleFillVec3s()\n";
        exit(1);
    }

    // make range for x & y rectangle(AABB box)
    float x_top, x_bottom;
    float y_top, y_bottom;
    x_top = x_bottom = tri_vec3s[0].v1;
    y_top = y_bottom = tri_vec3s[0].v2;

    for (int i = 1; i < 3; i++) {
        const float &tmp_x = tri_vec3s[i].v1;
        const float &tmp_y = tri_vec3s[i].v2;

        if (x_top < tmp_x)
            x_top = tmp_x;
        else if (x_bottom > tmp_x)
            x_bottom = tmp_x;

        if (y_top < tmp_y)
            y_top = tmp_y;
        else if (y_bottom > tmp_y)
            y_bottom = tmp_y;
    }

    // cast triangle vertexes to only 2D
    std::vector<Vector2> tri_vec2s;
    for (auto &vec3 : tri_vec3s) tri_vec2s.emplace_back(vec3.v1, vec3.v2);

    std::vector<Vector3> result;
    const int ceil_y_bottom = ceil(y_bottom);
    const int ceil_x_bottom = ceil(x_bottom);

    for (int i = floor(y_top); i >= ceil_y_bottom; i--)
        for (int j = floor(x_top); j >= ceil_x_bottom; j--) {
            // (j, i) = a*v1 + b*v2
            // a+b <=1 && a+b >=0 && a*b >=0 means in triangle
            Vector3 counter_origin = tri_vec3s[1];
            Vector3 v1 = tri_vec3s[0] - counter_origin;
            Vector3 v2 = tri_vec3s[2] - counter_origin;

            Vector2 ab;
            {

                /*
                    |x1 x2||a| |x3|
                    |y1 y2||b|=|y3|
                */
                Matrix2 mat2inv = Matrix2(v1.v1, v2.v1, v1.v2, v2.v2).inverse();
                Matrix2 mat2 = Matrix2(v1.v1, v2.v1, v1.v2, v2.v2);

                if (!mat2inv.isExist()) {  // change to another line
                    counter_origin = tri_vec3s[0];
                    v1 = tri_vec3s[1] - counter_origin;
                    v2 = tri_vec3s[2] - counter_origin;

                    mat2inv = Matrix2(v1.v1, v2.v1, v1.v2, v2.v2).inverse();
                    mat2 = Matrix2(v1.v1, v2.v1, v1.v2, v2.v2);

                    if (!mat2inv.isExist()) {
                        std::cerr << "mat2inv doesn't exist when triangle "
                                     "vertexes are\n";
                        for (auto &vec3 : tri_vec3s) std::cerr << vec3 << ", ";
                        std::cerr << "\n";
                        // exit(1);
                        return result;
                    }
                }

                const Vector2 v_test(j - counter_origin.v1,
                                     i - counter_origin.v2);
                ab = mat2inv * v_test;
            }

            float count_ab = ab.v1 + ab.v2;
            if (count_ab <= 1.001f && count_ab >= 0.0f && ab.v1 * ab.v2 >= 0)
                result.emplace_back(
                    j, i, counter_origin.v3 + ab.dot(Vector2(v1.v3, v2.v3)));

            // if (isInTriangle(tri_vec2s, Vector2(j, i))) {
            //     result.emplace_back(calcPlanePoint(tri_vec3s, Vector2(i,
            //     j))); std::cerr << "\tsuccess!!!\n";
            // }
        }

    return result;
}

// bool isInTriangle(const std::vector<Vector2> &tri_vec2s,
//                   const Vector2 &test_vec2)
// {
//     const Vector2 *last_vec2 = &tri_vec2s[tri_vec2s.size() - 1];
//     const Vector2 *now_vec2;

//     // auto last_it = tri_vec2s.end();
//     // last_it--;

//     for (int i = 0; i < tri_vec2s.size(); i++) {
//         now_vec2 = &tri_vec2s[i];

//         if (last_vec2->v2 == now_vec2->v2) {
//             if (last_vec2->v2 > test_vec2.v2) return false;
//         }
//         else if (Line(*last_vec2, *now_vec2).getValue(Point(test_vec2)) > 0)
//         {
//             std::cerr
//                 << "2\tLine(*last_it, *now_it).getValue(Point(test_vec2)) = "
//                 << Line(*last_vec2, *now_vec2).getValue(Point(test_vec2)) <<
//                 "\n";
//             // std::cerr << "*last_it: " << (*last_vec2)
//             //           << ", *now_it: " << (*now_vec2) << "\n";
//             return false;
//         }

//         now_vec2 = last_vec2;
//     }

//     return true;
// }

// Vector3 calcPlanePoint(const std::vector<Vector3> &vertexes,
//                        Vector2 xy)  // TODO. check later
// {
//     const Vector3 &origin = vertexes[1];
//     const Vector3 v1 = vertexes[0] - origin;
//     const Vector3 v2 = vertexes[2] - origin;
//     const Vector2 xy_counter_to_origin(xy.v1 - origin.v1, xy.v2 - origin.v2);

//     /*
//         |x1 x2||a| |x3|
//         |y1 y2||b|=|y3|
//     */
//     Matrix2 mat2inv;
//     {
//         float tmp_m[2][2];

//         tmp_m[0][0] = v1.v1;
//         tmp_m[0][1] = v2.v1;
//         tmp_m[1][0] = v1.v2;
//         tmp_m[1][1] = v2.v2;

//         mat2inv = Matrix2(tmp_m).inverse();
//     }

//     if (!mat2inv.isExist()) {
//         std::cerr << "Counter a triangle cannot count the plane point\n";
//         exit(1);
//     }

//     Vector2 ab = mat2inv * xy_counter_to_origin;

//     return Vector3(xy.v1, xy.v2, ab.dot(Vector2(v1.v3, v2.v3)) + origin.v3);
// }

std::vector<Vector3> Poly3D::getFillVec3s() const
{
    auto triangles = this->toTriangles();

    // std::cerr << "Split triangles: " << triangles.size() << "\n";
    // for (auto &tri : triangles) {
    //     static int count = 1;
    //     std::cerr << "The " << count++ << " triangle:\n\t";
    //     for (auto &ver : tri) std::cerr << ver << ", ";
    //     std::cerr << "\n";
    // }

    std::vector<Vector3> fillVec3s;

    for (auto &tri : triangles)
        for (auto &tri_vec3 : getTriangleFillVec3s(tri))
            fillVec3s.push_back(tri_vec3);

    return fillVec3s;
}
