#include "Scene.h"

#include <cmath>
#include <vector>

#include "GRGB.h"
#include "Math/Util.h"
#include "Math/Vector.h"
#include "Shape/Line.h"
#include "Shape/Point.h"
#include "Shape/Poly.h"
#include "Transform.h"

Transform2D Scene::getViewportTransform() const
{
    Vector2 tmp_tran((1.0f + this->vp.vxl) * this->win_width / 2,
                     (1.0f + this->vp.vyb) * this->win_width / 2);
    Vector2 tmp_scal((this->vp.vxr - this->vp.vxl) * this->win_width / 4,
                     (this->vp.vyt - this->vp.vyb) * this->win_width / 4);

    Transform2D tr;
    std::cerr << "To screen:\n";  // TODO.
    tr.translate(Vector2(1.0f, 1.0f));
    tr.scale(tmp_scal);
    tr.translate(tmp_tran);

    return tr;
}

void Scene::drawViewport(const Transform2D &tr) const
{
    Point lb = tr.getResult(Point(-1.0f, -1.0f));
    Point lt = tr.getResult(Point(-1.0f, 1.0f));
    Point rb = tr.getResult(Point(1.0f, -1.0f));
    Point rt = tr.getResult(Point(1.0f, 1.0f));

    std::cerr << lb << " " << lt << " " << rb << " " << rt << std::endl;

    Line(lb, lt, GRGB(1.0f, 1.0f, 1.0f)).draw();
    Line(lt, rt, GRGB(1.0f, 1.0f, 1.0f)).draw();
    Line(rt, rb, GRGB(1.0f, 1.0f, 1.0f)).draw();
    Line(rb, lb, GRGB(1.0f, 1.0f, 1.0f)).draw();
}

std::vector<Vector4> cutFace(std::vector<Vector4> vec4s)
{
    // cut line
    std::vector<Vector4> tmp_vec4s;
    Vector4 *last_vec4, *now_vec4;
    float last_c, now_c;

    for (int plane_i = 0; plane_i < 6; plane_i++) {
        last_vec4 = &vec4s[vec4s.size() - 1];
        last_c = getPlaneC(plane_i, last_vec4);

        for (int i = 0; i < vec4s.size(); i++) {
            now_vec4 = &vec4s[i];
            now_c = getPlaneC(plane_i, now_vec4);

            if (now_c >= 0) {
                if (last_c < 0) {
                    Vector4 *tmp_vec4 =
                        calcPlaneVec4(plane_i, last_vec4, now_vec4, last_c, now_c);

                    if (tmp_vec4 != nullptr) tmp_vec4s.push_back(*tmp_vec4);

                    delete tmp_vec4;
                }
                tmp_vec4s.push_back(*now_vec4);
            }
            else {
                Vector4 *tmp_vec4 =
                    calcPlaneVec4(plane_i, last_vec4, now_vec4, last_c, now_c);

                if (tmp_vec4 != nullptr) tmp_vec4s.push_back(*tmp_vec4);

                delete tmp_vec4;
            }

            last_vec4 = now_vec4;
            last_c = now_c;
        }

        vec4s = tmp_vec4s;
        tmp_vec4s.clear();
    }

    return vec4s;
}

float getPlaneC(const int plane_i, const Vector4 *vec4)
{
    const float &x = vec4->v1;
    const float &y = vec4->v2;
    const float &z = vec4->v3;
    const float &w = vec4->v4;

    switch (plane_i) {
        case 0: {
            return w + x;
        }
        case 1: {
            return w - x;
        }
        case 2: {
            return w + y;
        }
        case 3: {
            return w - y;
        }
        case 4: {
            return z;
        }
        case 5: {
            return w - z;
        }
        default: {
            std::cerr << "plane_i index error\n";
            exit(1);
        }
    }
}

Vector4 *calcPlaneVec4(const float plane_i, const Vector4 *last_vec4, const Vector4 *now_vec4,
                       const float last_c, const float now_c)
{
    Vector4 *tmp_vec4 = new Vector4();

    *tmp_vec4 = *last_vec4;

    if (last_c != now_c) {
        float t = last_c / (last_c - now_c);

        *tmp_vec4 += (*now_vec4 - *last_vec4) * t;

        return tmp_vec4;
    }
    else {
        std::cerr << "calculate plane vector4 on plane_i: " << plane_i
                  << "error (line " << *last_vec4 << " to " << *now_vec4
                  << ")\n";
        return nullptr;
    }
}

void Scene::show(const Camera &cam) const
{
    Transform3D tr_em = cam.getEM();  // object's transform
    Transform3D tr_pm = cam.getPM(this->getAspectRatio());
    Transform2D tr_view = this->getViewportTransform();

    this->drawViewport(tr_view);

    for (GObj *ol : this->layers) {  // for each .obj
        Transform3D tr_tm = ol->getTM();

        // stderr
        {
            std::cerr << "Obj: " << ol->getFileName() << "\n";
            std::cerr << "M Matrix:\n";
            tr_tm.print();
            std::cerr << "V Matrix:\n";
            tr_em.print();
            std::cerr << "P Matrix:\n";
            tr_pm.print();
        }

        for (Face &face : ol->getLoadedFaces()) {  // for each triangle
            std::vector<Vector3> vecs;

            for (Vector3 &ver : face.vertexes)  // for each vertex
                vecs.emplace_back(ver.v1, ver.v2, ver.v3);

            // tm
            {
                std::vector<Vector3> tmp_vecs;

                for (auto &vec : vecs)
                    tmp_vecs.emplace_back(tr_tm.getResult(vec));

                vecs = tmp_vecs;
            }
            // em
            {
                std::vector<Vector3> tmp_vecs;

                for (auto &vec : vecs)
                    tmp_vecs.emplace_back(tr_em.getResult(vec));

                vecs = tmp_vecs;
            }
            // pm
            std::vector<Vector4> vec4s;
            {
                for (auto &vec : vecs)
                    vec4s.emplace_back(tr_pm.getResult(Vector4(vec, 1)));
            }

            // vec4s = cutFace(vec4s); // TODO. error

            // perspective divide
            {
                vecs.clear();

                for (auto &vec : vec4s)
                    vecs.emplace_back(this->perspectiveDivide(vec));
            }
            // win to view
            {
                Point last_p =
                    tr_view.getResult(Point(vecs.back().v1, vecs.back().v2));

                for (auto &vec : vecs) {
                    Point now_p = tr_view.getResult(Point(vec.v1, vec.v2));

                    Line(last_p, now_p, this->default_rgb).draw();

                    last_p = now_p;
                }
            }
        }
    }
}

void Scene::clear()
{
    for (auto *ol : this->layers) {
        auto *tmp_ol = ol;
        ol = nullptr;
        delete tmp_ol;
    }
    this->layers.clear();
}
