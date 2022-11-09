#include "Scene.h"

#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

#include "BasicGraph.h"
#include "Line.h"
#include "Point.h"
#include "Poly.h"
#include "TransformMatrix.h"

Scene::Scene() { return; }

void Scene::addLayer(Poly *bg)
{
    this->layer[this->layer.size()] = bg;
    return;
}

void Scene::show(float scene_left, float scene_right, float scene_bottom, float scene_top, float window_left,
                 float window_right, float window_bottom, float window_top)
{
    // 1. error checking
    if ((scene_right - scene_left) == 0 || (scene_top - scene_bottom) == 0) {
        std::cerr << "Input error window view size\n";
        return;
    }

    // 2. make view transform
    TransformMatrix result_tr;
    result_tr.translate(-scene_left, -scene_bottom, false);
    result_tr.scale((window_right - window_left) / (scene_right - scene_left),
                    (window_top - window_bottom) / (scene_top - scene_bottom), false);
    result_tr.translate(window_left, window_bottom, false);

    // 3. drawing window's edge
    Poly(std::vector<Point>({Point(window_right, window_top), Point(window_left, window_top),
                             Point(window_left, window_bottom), Point(window_right, window_bottom)}),
         RGBCode(1.0f, 1.0f, 1.0f), DEFAULT_SIZE)
        .draw();

    // 4. cut edge and add to printing_graph
    std::vector<Poly *> printing_graph;

    for (auto &bg : this->layer) {
        std::cerr << "---for each poly---\n";
        // (1) init
        // Poly *tmp_poly = dynamic_cast<Poly *>(bg.second);
        Poly *tmp_poly = bg.second;
        std::vector<Point> poly_points = tmp_poly->getPoints();

        // test
        // std::cerr << "View adding point each poly: ";
        // std::cerr << "(" << now_p->getX() << ":" << now_p->getY() << "), ";
        // !test

        // (2) cut with the Line(*last_p, *now_p) & *now_p
        // top
        Point *last_p = &poly_points[poly_points.size() - 1];
        std::vector<Point> tmp_poly_points;

        for (int i = 0; i < poly_points.size(); i++) {
            Point *now_p = &poly_points[i];

            bool last_posi = (last_p->getY() <= scene_top);  // 1:in, 0:out
            bool now_posi = (now_p->getY() <= scene_top);

            if (last_posi && now_posi)
                tmp_poly_points.push_back(*now_p);
            else if (last_posi ^ now_posi) {  // xor
                Point tmp_p = Line(*last_p, *now_p).calYCrossPoint(scene_top);

                // last point has been deal with if in(including on line), sod don't double added
                if (last_p->getY() != scene_top) tmp_poly_points.push_back(tmp_p);

                // now_posi is in, so just add when *now_p isn't on the line
                if (now_p->getY() < scene_top) tmp_poly_points.push_back(*now_p);
            }

            last_p = now_p;
        }

        poly_points = tmp_poly_points;

        if (!poly_points.size()) continue;
        for(auto& p:poly_points )
            std::cerr << "Points: (" << p.getX() << ", " << p.getY() << "), ";
        std::cerr << "\n";

        // left
        last_p = &poly_points[poly_points.size() - 1];
        tmp_poly_points.clear();

        for (int i = 0; i < poly_points.size(); i++) {
            Point *now_p = &poly_points[i];

            bool last_posi = last_p->getX() >= scene_left;  // 1:in, 0:out
            bool now_posi = now_p->getX() >= scene_left;

            if (last_posi && now_posi)
                tmp_poly_points.push_back(*now_p);
            else if (last_posi ^ now_posi) {  // xor
                Point tmp_p = Line(*last_p, *now_p).calXCrossPoint(scene_left);

                // last point has been deal with if in(including on line), sod don't double added
                if (last_p->getX() != scene_left) tmp_poly_points.push_back(tmp_p);

                // now_posi is in, so just add when *now_p isn't on the line
                if (now_p->getX() > scene_left) tmp_poly_points.push_back(*now_p);
            }

            last_p = now_p;
        }

        poly_points = tmp_poly_points;

        if (!poly_points.size()) continue;
        for(auto& p:poly_points )
            std::cerr << "Points: (" << p.getX() << ", " << p.getY() << "), ";
        std::cerr << "\n";

        // bottom
        last_p = &poly_points[poly_points.size() - 1];
        tmp_poly_points.clear();

        for (int i = 0; i < poly_points.size(); i++) {
            Point *now_p = &poly_points[i];

            bool last_posi = last_p->getY() >= scene_bottom;  // 1:in, 0:out
            bool now_posi = now_p->getY() >= scene_bottom;

            if (last_posi && now_posi)
                tmp_poly_points.push_back(*now_p);
            else if (last_posi ^ now_posi) {  // xor
                Point tmp_p = Line(*last_p, *now_p).calYCrossPoint(scene_bottom);

                // last point has been deal with if in(including on line), sod don't double added
                if (last_p->getY() != scene_bottom) tmp_poly_points.push_back(tmp_p);

                // now_posi is in, so just add when *now_p isn't on the line
                if (now_p->getY() > scene_bottom) tmp_poly_points.push_back(*now_p);
            }

            last_p = now_p;
        }

        poly_points = tmp_poly_points;

        if (!poly_points.size()) continue;
        for(auto& p:poly_points )
            std::cerr << "Points: (" << p.getX() << ", " << p.getY() << "), ";
        std::cerr << "\n";

        // right
        last_p = &poly_points[poly_points.size() - 1];
        tmp_poly_points.clear();

        for (int i = 0; i < poly_points.size(); i++) {
            Point *now_p = &poly_points[i];

            bool last_posi = last_p->getX() <= scene_right;  // 1:in, 0:out
            bool now_posi = now_p->getX() <= scene_right;

            if (last_posi && now_posi)
                tmp_poly_points.push_back(*now_p);
            else if (last_posi ^ now_posi) {  // xor
                Point tmp_p = Line(*last_p, *now_p).calXCrossPoint(scene_right);

                // last point has been deal with if in(including on line), sod don't double added
                if (last_p->getX() != scene_right) tmp_poly_points.push_back(tmp_p);

                // now_posi is in, so just add when *now_p isn't on the line
                if (now_p->getX() < scene_right) tmp_poly_points.push_back(*now_p);
            }

            last_p = now_p;
        }

        poly_points = tmp_poly_points;

        if (!poly_points.size()) continue;
        for(auto& p:poly_points )
            std::cerr << "Points: (" << p.getX() << ", " << p.getY() << "), ";
        std::cerr << "\n";

        // (3) add to printing_graph
        Poly *tmp_to_graph = new Poly(poly_points, tmp_poly->getRGBCode(), tmp_poly->getEdgeSize());
        printing_graph.push_back(tmp_to_graph);
    }

    // 5. print to screen
    // std::cerr << "Poly_size"
    for (Poly *p : printing_graph) {
        // (1) add view transform
        p->addTransform(result_tr);

        // (2) print
        // p->draw();
        p->fill();

        // (3) clear printing_graph's pointer
        Poly *tmp_p = p;
        p = nullptr;
        delete tmp_p;
    }

    return;
}

void Scene::clear()
{
    for (auto &bg : this->layer) {
        Poly *tmp_bg = bg.second;
        bg.second = nullptr;
        delete tmp_bg;
    }
    this->layer.clear();
    return;
}