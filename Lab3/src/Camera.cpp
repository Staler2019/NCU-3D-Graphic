#include "Camera.h"

#include <iostream>
#include <vector>

#include "Math/Vector.h"
#include "Shape/Point.h"
#include "Shape/Poly.h"

Poly* Camera::castToHither(const std::vector<Vector3>& mesh_points, const GRGB& default_rgb) const{
    // cut edge and add to printing_graph
    std::cerr << "---for each graph to near---\n";
    std::vector<Vector3> vec_points = mesh_points;
    // Vector3's
    // make all lines before Far plane
    Vector3* last_vec = &vec_points[vec_points.size() - 1];
    std::vector<Vector3> tmp_vec_points;

    for (int i = 0; i < vec_points.size(); i++) {
        Vector3* now_vec = &vec_points[i];

        int last_posi;
        int now_posi;

        if(last_vec->v3 < this->Hither) // too close
            last_posi = 0;
        else if(last_vec->v3 > this->Yon) // too far
            last_posi = 2;
        else // in
            last_posi = 1;

        if(now_vec->v3 < this->Hither) // too close
            now_posi = 0;
        else if(now_vec->v3 > this->Yon) // too far
            now_posi = 2;
        else // in
            now_posi = 1;

        if (last_posi == 1 && now_posi == 1)
            tmp_vec_points.push_back(*now_vec);
        if(last_posi == 1){
            if()
        }
        else if (last_posi == 0){
            if(now_posi == 0){}
            else {
                Vector3 tmp_vec = Line3D(*last_vec, *now_vec).calZCrossPoint(this->Hither);

                if(tmp_vec.v3 != this->Hither)
            }

        } else if (last_posi == 2){

        }



        else if (last_posi ^ now_posi) {  // xor
            Vector3 tmp_vec = Line3D(*last_vec, *now_vec).calZCrossPoint(this->Yon);
            Point tmp_p = Line(*last_p, *now_p).calYCrossPoint(top_edge);

            // last point has been deal with if in(including on line), sod don't double added
            if (last_p->getY() != top_edge) tmp_poly_points.push_back(tmp_p);

            // now_posi is in, so just add when *now_p isn't on the line
            if (now_p->getY() < top_edge) tmp_poly_points.push_back(*now_p);
        }

        last_p = now_p;
    }

    poly_points = tmp_poly_points;

    if (!poly_points.size()) return nullptr;
    for (auto& p : poly_points) std::cerr << "Points: (" << p.getX() << ", " << p.getY() << "), ";
    std::cerr << "\n";






    // (1) init
    std::vector<Point> poly_points = poly->getPoints();

    // (1-1)




    // test
    // std::cerr << "View adding point each poly: ";
    // std::cerr << "(" << now_p->getX() << ":" << now_p->getY() << "), ";
    // !test

    // (2) cut with the Line(*last_p, *now_p) & *now_p
    // top
    const float &top_edge = hither_t;
    Point* last_p = &poly_points[poly_points.size() - 1];
    std::vector<Point> tmp_poly_points;

    for (int i = 0; i < poly_points.size(); i++) {
        Point* now_p = &poly_points[i];

        bool last_posi = (last_p->getY() <= top_edge);  // 1:in, 0:out
        bool now_posi = (now_p->getY() <= top_edge);

        if (last_posi && now_posi)
            tmp_poly_points.push_back(*now_p);
        else if (last_posi ^ now_posi) {  // xor
            Point tmp_p = Line(*last_p, *now_p).calYCrossPoint(top_edge);

            // last point has been deal with if in(including on line), sod don't double added
            if (last_p->getY() != top_edge) tmp_poly_points.push_back(tmp_p);

            // now_posi is in, so just add when *now_p isn't on the line
            if (now_p->getY() < top_edge) tmp_poly_points.push_back(*now_p);
        }

        last_p = now_p;
    }

    poly_points = tmp_poly_points;

    if (!poly_points.size()) return nullptr;
    for (auto& p : poly_points) std::cerr << "Points: (" << p.getX() << ", " << p.getY() << "), ";
    std::cerr << "\n";

    // left
    const float &left_edge = hither_l;
    last_p = &poly_points[poly_points.size() - 1];
    tmp_poly_points.clear();

    for (int i = 0; i < poly_points.size(); i++) {
        Point* now_p = &poly_points[i];

        bool last_posi = last_p->getX() >= left_edge;  // 1:in, 0:out
        bool now_posi = now_p->getX() >= left_edge;

        if (last_posi && now_posi)
            tmp_poly_points.push_back(*now_p);
        else if (last_posi ^ now_posi) {  // xor
            Point tmp_p = Line(*last_p, *now_p).calXCrossPoint(left_edge);

            // last point has been deal with if in(including on line), sod don't double added
            if (last_p->getX() != left_edge) tmp_poly_points.push_back(tmp_p);

            // now_posi is in, so just add when *now_p isn't on the line
            if (now_p->getX() > left_edge) tmp_poly_points.push_back(*now_p);
        }

        last_p = now_p;
    }

    poly_points = tmp_poly_points;

    if (!poly_points.size()) return nullptr;
    for (auto& p : poly_points) std::cerr << "Points: (" << p.getX() << ", " << p.getY() << "), ";
    std::cerr << "\n";

    // bottom
    const float &bottom_edge = hither_b;
    last_p = &poly_points[poly_points.size() - 1];
    tmp_poly_points.clear();

    for (int i = 0; i < poly_points.size(); i++) {
        Point* now_p = &poly_points[i];

        bool last_posi = last_p->getY() >= bottom_edge;  // 1:in, 0:out
        bool now_posi = now_p->getY() >= bottom_edge;

        if (last_posi && now_posi)
            tmp_poly_points.push_back(*now_p);
        else if (last_posi ^ now_posi) {  // xor
            Point tmp_p = Line(*last_p, *now_p).calYCrossPoint(bottom_edge);

            // last point has been deal with if in(including on line), sod don't double added
            if (last_p->getY() != bottom_edge) tmp_poly_points.push_back(tmp_p);

            // now_posi is in, so just add when *now_p isn't on the line
            if (now_p->getY() > bottom_edge) tmp_poly_points.push_back(*now_p);
        }

        last_p = now_p;
    }

    poly_points = tmp_poly_points;

    if (!poly_points.size()) return nullptr;
    for (auto& p : poly_points) std::cerr << "Points: (" << p.getX() << ", " << p.getY() << "), ";
    std::cerr << "\n";

    // right
    const float &right_edge = hither_right;
    last_p = &poly_points[poly_points.size() - 1];
    tmp_poly_points.clear();

    for (int i = 0; i < poly_points.size(); i++) {
        Point* now_p = &poly_points[i];

        bool last_posi = last_p->getX() <= right_edge;  // 1:in, 0:out
        bool now_posi = now_p->getX() <= right_edge;

        if (last_posi && now_posi)
            tmp_poly_points.push_back(*now_p);
        else if (last_posi ^ now_posi) {  // xor
            Point tmp_p = Line(*last_p, *now_p).calXCrossPoint(right_edge);

            // last point has been deal with if in(including on line), sod don't double added
            if (last_p->getX() != right_edge) tmp_poly_points.push_back(tmp_p);

            // now_posi is in, so just add when *now_p isn't on the line
            if (now_p->getX() < right_edge) tmp_poly_points.push_back(*now_p);
        }

        last_p = now_p;
    }

    poly_points = tmp_poly_points;

    if (!poly_points.size()) return nullptr;
    for (auto& p : poly_points) std::cerr << "Points: (" << p.getX() << ", " << p.getY() << "), ";
    std::cerr << "\n";

    // (3) add to printing_graph
    return new Poly(poly_points, default_rgb, 1.0f);
}