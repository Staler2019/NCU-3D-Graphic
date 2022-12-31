#include "Scene.h"

#include <algorithm>
#include <cmath>
#include <vector>

#include "GLInclude.h"
#include "GRGB.h"
#include "Math/Util.h"
#include "Math/Vector.h"
#include "Shape/Line.h"
#include "Shape/Point.h"
#include "Shape/Poly.h"
#include "Transform.h"

Transform2D Scene3D::getViewportTransform() const
{
    Vector2 tmp_tran((1.0f + this->vp.vxl) * this->win_width / 2,
                     (1.0f + this->vp.vyb) * this->win_width / 2);
    Vector2 tmp_scal((this->vp.vxr - this->vp.vxl) * this->win_width / 4,
                     (this->vp.vyt - this->vp.vyb) * this->win_width / 4);

    Transform2D tr;

    tr.translate(Vector2(1.0f, 1.0f), false);
    tr.scale(tmp_scal, false);
    tr.translate(tmp_tran, false);

    return tr;
}

void Scene3D::drawViewport(const Transform2D &tr) const
{
    Point lb = tr.getResult(Point(-1.0f, -1.0f));
    Point lt = tr.getResult(Point(-1.0f, 1.0f));
    Point rb = tr.getResult(Point(1.0f, -1.0f));
    Point rt = tr.getResult(Point(1.0f, 1.0f));

    std::cerr << "Viewport 4 points: " << lb << " " << lt << " " << rb << " "
              << rt << std::endl;

    Line(lb, lt, GRGB(1.0f, 1.0f, 1.0f)).draw();
    Line(lt, rt, GRGB(1.0f, 1.0f, 1.0f)).draw();
    Line(rt, rb, GRGB(1.0f, 1.0f, 1.0f)).draw();
    Line(rb, lb, GRGB(1.0f, 1.0f, 1.0f)).draw();
}

void cutFace(std::vector<Vector4> &vec4s)
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
                if (last_c < 0 && now_c > 0) {
                    Vector4 *tmp_vec4 = calcPlaneVec4(plane_i, last_vec4,
                                                      now_vec4, last_c, now_c);

                    if (tmp_vec4 != nullptr) tmp_vec4s.push_back(*tmp_vec4);

                    delete tmp_vec4;
                }
                tmp_vec4s.push_back(*now_vec4);
            }
            else {
                if (last_c > 0) {
                    Vector4 *tmp_vec4 = calcPlaneVec4(plane_i, last_vec4,
                                                      now_vec4, last_c, now_c);

                    if (tmp_vec4 != nullptr) tmp_vec4s.push_back(*tmp_vec4);

                    delete tmp_vec4;
                }
            }

            last_vec4 = now_vec4;
            last_c = now_c;
        }

        vec4s = tmp_vec4s;
        tmp_vec4s.clear();

        if (vec4s.empty()) return;
    }
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

Vector4 *calcPlaneVec4(const float plane_i, const Vector4 *last_vec4,
                       const Vector4 *now_vec4, const float last_c,
                       const float now_c)
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

void Scene3D::showLines(const Camera &cam) const
{
    Transform3D tr_em = cam.getEM();  // object's transform
    Transform3D tr_pm = cam.getPM();
    Transform3D tr_VP = tr_pm * tr_em;
    Transform2D tr_view = this->getViewportTransform();

    for (GObj *ol : this->layers) {  // for each .obj
        Transform3D tr_tm = ol->getTM();
        Transform3D tr_MVP = tr_VP * tr_tm;

        // stderr
        {
            std::cerr << "Obj: " << ol->getFileName() << "\n";
            std::cerr << "M Matrix:\n";
            tr_tm.print();
            std::cerr << "V Matrix:\n";
            tr_em.print();
            std::cerr << "P Matrix:\n";
            tr_pm.print();
            std::cerr << "View to Screem Matrix:\n";
            tr_view.printMat();
        }

        for (Face &face : ol->getFaces()) {  // for each triangle
            std::vector<Vector4> vec4s;

            for (Vector3 *vec3 : face.Vs)
                vec4s.emplace_back(tr_MVP.getResult(Vector4(*vec3, 1)));

            // cut face
            {
                cutFace(vec4s);

                if (vec4s.empty()) continue;
            }
            // perspective divide
            std::vector<Vector3> vec3s;
            {
                for (auto &vec : vec4s)
                    vec3s.emplace_back(this->perspectiveDivide(vec));
            }
            // win to view
            {
                Point last_p =
                    tr_view.getResult(Point(vec3s.back().v1, vec3s.back().v2));

                for (auto &vec : vec3s) {
                    Point now_p = tr_view.getResult(Point(vec.v1, vec.v2));

                    Line(last_p, now_p, this->default_rgb).draw();

                    last_p = now_p;
                }
            }
        }
    }

    this->drawViewport(tr_view);
}

void Scene3D::showTextures(const Camera &cam) const
{
    Transform3D tr_em = cam.getEM();  // object's transform
    Transform3D tr_pm = cam.getPM();
    Transform3D tr_VP = tr_pm * tr_em;
    Transform2D tr_view = this->getViewportTransform();

    auto z_buffer = this->initZBuffer();
    auto c_buffer = this->initCBuffer(tr_view);

    std::cerr << "Number of shapes: " << this->layers.size() << "\n";

    for (GObj *ol : this->layers) {  // for each .obj
        // bool debug_mode = false;
        // unsigned long long face_counter = 0;

        // std::cout << "\nInput c/C if you want to debug into a shape: ";
        // char check_char = fgetc(stdin);
        // if (check_char == 'c' || check_char == 'C') debug_mode = true;
        // std::cout << "\n";

        Transform3D tr_tm = ol->getTM();
        Transform3D tr_MVP = tr_VP * tr_tm;

        // stderr
        {
            std::cerr << "Obj: " << ol->getFileName() << " with "
                      << ol->getFaces().size() << " faces"
                      << "\n";
            std::cerr << "M Matrix:\n";
            tr_tm.print();
            std::cerr << "V Matrix:\n";
            tr_em.print();
            std::cerr << "P Matrix:\n";
            tr_pm.print();
            std::cerr << "View to Screem Matrix:\n";
            tr_view.printMat();
        }

        for (Face &face : ol->getFaces()) {  // for each triangle
            std::vector<Vector4> vec4s;
            std::vector<Vector3> vec3s;
            GRGB flat_shading;

            // light
            {
                for (Vector3 *vec3 : face.Vs)
                    vec3s.emplace_back(tr_tm.getResult(*vec3));

                auto l = [](const Vector3 &a, const Vector3 &b) {
                    return a.cross(b).norm();
                };

                // ERROR: norm need to be transform by rotate only
                // Vector3 normal =
                //     (face.VNs[0] != nullptr)
                //         ? (*face.VNs[0])
                //         : l(vec3s[1] - vec3s[0], vec3s[2] - vec3s[1]);

                Vector3 normal = l(vec3s[1] - vec3s[0], vec3s[2] - vec3s[1]);

                flat_shading =
                    this->calcLight(*face.Vs[0], normal, cam.posi, tr_tm,
                                    ol->rgb, ol->Kd, ol->Ks, ol->N, true);

                vec3s.clear();
            }
            // MVP matrix
            {
                for (Vector3 *vec3 : face.Vs)
                    vec4s.emplace_back(tr_MVP.getResult(Vector4(*vec3, 1)));
            }
            // cut face
            {
                cutFace(vec4s);

                if (vec4s.empty()) continue;
            }
            // perspective divide
            {
                for (auto &vec : vec4s)
                    vec3s.emplace_back(this->perspectiveDivide(vec));
            }
            // win to view: z buffer & c buffer
            {
                std::vector<Vector3> tmp_vec3s;

                for (auto &vec : vec3s) {
                    Point tmp_p = tr_view.getResult(Point(vec.v1, vec.v2));
                    tmp_vec3s.emplace_back(tmp_p.getX(), tmp_p.getY(), vec.v3);
                }

                std::vector<Vector3> poly_fill_points =
                    Poly3D(tmp_vec3s).getFillVec3s();

                for (auto &tmp_vec3 : poly_fill_points) {
                    const int &tmp_x = gFloor(tmp_vec3.v1);
                    const int &tmp_y = gFloor(tmp_vec3.v2);
                    const float &tmp_z = tmp_vec3.v3;

                    if (tmp_z < z_buffer[tmp_y][tmp_x]) {
                        z_buffer[tmp_y][tmp_x] = tmp_z;
                        c_buffer[tmp_y][tmp_x] = flat_shading;
                    }
                }
            }
            // if (debug_mode) {
            //     const int face_unit = 1;

            //     if (++face_counter % face_unit == 0) {
            //         this->drawBuffer(c_buffer);

            //         {
            //             std::vector<Vector3> tmp_vec3s;

            //             for (auto &vec : vec3s) {
            //                 Point tmp_p =
            //                     tr_view.getResult(Point(vec.v1, vec.v2));
            //                 tmp_vec3s.emplace_back(tmp_p.getX(),
            //                 tmp_p.getY(),
            //                                        vec.v3);
            //             }

            //             Vector3 *last_vec3 = &tmp_vec3s[tmp_vec3s.size() -
            //             1]; Vector3 *now_vec3;

            //             std::cerr << "Poly vertexes: \n";
            //             for (auto &a : tmp_vec3s) {
            //                 now_vec3 = &a;

            //                 std::cerr << a << ", ";
            //                 Point(a.v1, a.v2,
            //                 this->default_rgb, 5.0F).draw();
            //                 Line(Vector2(last_vec3->v1, last_vec3->v2),
            //                      Vector2(now_vec3->v1, now_vec3->v2),
            //                      this->default_rgb, 2.0f)
            //                     .draw();

            //                 last_vec3 = now_vec3;
            //             }
            //             std::cerr << std::endl;
            //         }

            //         glFlush();

            //         std::cerr << "----------" << face_counter << "/"
            //                   << face_unit << "-------------\n";
            //         std::cout << "please input something to next face\n";
            //         fgetc(stdin);
            //     }
            // }
        }
        // this->drawBuffer(c_buffer);
        // glFlush();
    }

    this->drawBuffer(c_buffer);

    // this->drawViewport(tr_view);
}

void Scene3D::clear()
{
    for (auto *ol : this->layers) {
        auto *tmp_ol = ol;
        ol = nullptr;
        delete tmp_ol;
    }
    this->layers.clear();

    if (!this->layers.empty()) {
        std::cerr << "layer clear error\n";
        exit(1);
    }
}

std::vector<std::vector<float>> Scene3D::initZBuffer() const
{
    // after perspective divide so use 1.0f
    return std::vector<std::vector<float>>(
        this->win_height + 1, std::vector<float>(this->win_width + 1, 1.0f));
}

std::vector<std::vector<GRGB>> Scene3D::initCBuffer(const Transform2D &tr) const
{
    std::vector<std::vector<GRGB>> c_buffer(
        this->win_height + 1,
        std::vector<GRGB>(this->win_width + 1, GRGB(0.0f, 0.0f, 0.0f)));

    Point lb = tr.getResult(Point(-1.0f, -1.0f));
    Point rt = tr.getResult(Point(1.0f, 1.0f));

    for (int i = gFloor(lb.getY()); i < gFloor(rt.getY()); i++)
        for (int j = gFloor(lb.getX()); j < gFloor(rt.getY()); j++)
            c_buffer[i][j] = this->bg_rgb;

    return c_buffer;
}

void Scene3D::drawBuffer(const std::vector<std::vector<GRGB>> &c_buffer) const
{
    GLubyte *draw_buffer = new GLubyte[this->win_height * this->win_width * 3];

    for (int i = 0; i < this->win_height; i++) {
        for (int j = 0; j < this->win_width; j++) {
            draw_buffer[i * this->win_width * 3 + j * 3 + 0] =
                (unsigned char)(c_buffer[i][j].r * (float)255);
            draw_buffer[i * this->win_width * 3 + j * 3 + 1] =
                (unsigned char)(c_buffer[i][j].g * (float)255);
            draw_buffer[i * this->win_width * 3 + j * 3 + 2] =
                (unsigned char)(c_buffer[i][j].b * (float)255);
        }
    }

    glDrawPixels(this->win_width, this->win_height, GL_RGB, GL_UNSIGNED_BYTE,
                 draw_buffer);

    delete[] draw_buffer;
}

GRGB Scene3D::calcLight(Vector3 vec3_point, const Vector3 &normal,
                        Vector3 cam_posi, const Transform3D &tr_tm,
                        const GRGB &obj_grgb, const float Kd, const float Ks,
                        const int N, bool modeRV) const
{
    // init
    vec3_point = tr_tm.getResult(vec3_point);
    cam_posi = tr_tm.getResult(cam_posi);
    const Vector3 obj_rgb = obj_grgb.toVec3();
    Vector3 V = (cam_posi - vec3_point).norm();

    // lights
    Vector3 vec3_light_ambi = this->ambient_rgb.toVec3() * obj_rgb;
    Vector3 vec3_light_diff;
    Vector3 vec3_light_spec;

    // fatt_rate
    float fatt_rate = 1;

    // for each spot light
    for (auto &light_pair : this->lights) {
        const Light &light = light_pair.second;
        Vector3 light_rgb = light.rgb.toVec3();

        Vector3 L = (tr_tm.getResult(light.posi) - vec3_point).norm();
        Vector3 H = (L + V).norm();

        float normal_dot_L = normal.dot(L);
        Vector3 R = normal * (2 * normal.dot(L)) - L;
        float R_dot_V = (modeRV) ? R.dot(V) : H.dot(normal);
        // float RdotV_powN = pow(std::max(H.dot(normal), 0.0f), N);
        // std::cerr << "RdotV_powN: " << RdotV_powN <<
        // "\n";

        if (normal_dot_L > 0)
            vec3_light_diff +=
                light_rgb * obj_rgb * (fatt_rate * Kd * normal_dot_L);

        if (R_dot_V > 0)
            vec3_light_spec += light_rgb * (fatt_rate * Ks * pow(R_dot_V, N));
    }

    // total the light color
    Vector3 tmp_flat_shading =
        vec3_light_ambi + vec3_light_diff + vec3_light_spec;
    // std::cerr << "light_ambi: " << vec3_light_ambi
    //           << ", light_diff: " << vec3_light_diff
    //           << ", light_spec: " << vec3_light_spec <<
    //           "\n";
    // std::cerr << "Now color: " << tmp_flat_shading <<
    // "\n";

    return GRGB(tmp_flat_shading).norm();  // max to 1, min to 0
}