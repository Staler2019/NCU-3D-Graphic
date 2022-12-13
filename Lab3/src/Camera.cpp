#include "Camera.h"

#include <cmath>
#include <iostream>
#include <vector>

#include "Math/Util.h"
#include "Math/Vector.h"
#include "Shape/Line.h"
#include "Shape/Point.h"
#include "Shape/Poly.h"
#include "Transform.h"

// Poly *Camera::castToHither(const std::vector<Vector3> &mesh_points,
//                            const GRGB &default_rgb) const {
//     std::cerr << "---for each graph to near---\n";
//     std::vector<Vector3> vec_points = mesh_points;
//     // Vector3's
//     // make all lines before Far plane
//     Vector3 *last_vec = &vec_points[vec_points.size() - 1];
//     int last_posi = this->checkPosi(last_vec->v3);
//     std::vector<Vector3> tmp_vec_points;
//
//     for (int i = 0; i < vec_points.size(); i++) {
//         Vector3 *now_vec = &vec_points[i];
//
//         int now_posi = this->checkPosi(now_vec->v3);
//
//         switch (last_posi) {
//             case 0: {
//                 switch (now_posi) {
//                     // no case 0
//                     case 1: {
//                         tmp_vec_points.push_back(
//                                 Line3D(*last_vec, *now_vec).calZCrossVec(
//                                         this->Hither));
//                         tmp_vec_points.push_back(*now_vec);
//                         break;
//                     }
//                     case 2: {
//                         tmp_vec_points.push_back(
//                                 Line3D(*last_vec, *now_vec).calZCrossVec(
//                                         this->Hither));
//                         tmp_vec_points.push_back(
//                                 Line3D(*last_vec, *now_vec).calZCrossVec(
//                                         this->Yon));
//                         break;
//                     }
//                 }
//                 break;
//             }
//             case 1: {
//                 switch (now_posi) {
//                     case 0: {
//                         tmp_vec_points.push_back(
//                                 Line3D(*last_vec, *now_vec).calZCrossVec(
//                                         this->Hither));
//                         break;
//                     }
//                     case 1: {
//                         tmp_vec_points.push_back(*now_vec);
//                         break;
//                     }
//                     case 2: {
//                         tmp_vec_points.push_back(
//                                 Line3D(*last_vec, *now_vec).calZCrossVec(
//                                         this->Yon));
//                         break;
//                     }
//                 }
//                 break;
//             }
//             case 2: {
//                 switch (now_posi) {
//                     case 0: {
//                         tmp_vec_points.push_back(
//                                 Line3D(*last_vec, *now_vec).calZCrossVec(
//                                         this->Yon));
//                         tmp_vec_points.push_back(
//                                 Line3D(*last_vec, *now_vec).calZCrossVec(
//                                         this->Hither));
//                         break;
//                     }
//                     case 1: {
//                         tmp_vec_points.push_back(
//                                 Line3D(*last_vec, *now_vec).calZCrossVec(
//                                         this->Yon));
//                         tmp_vec_points.push_back(*now_vec);
//                         break;
//                     }
//                         // no case 2
//                 }
//                 break;
//             }
//         }
//
//         last_posi = now_posi;
//         last_vec = now_vec;
//     }
//
//     vec_points = tmp_vec_points;
//
//     if (!vec_points.size()) return nullptr;
//     std::cerr << "Vectors: \n";
//     for (auto &v: vec_points) std::cerr << v << ", ";
//     std::cerr << "\n";
//
//     std::vector<Point> poly_points;
//
//     for (auto &vec: vec_points)
//         poly_points.push_back(this->vec3ToHither(vec));
//
//     return new Poly(poly_points, default_rgb, 1.0f);
// }

Transform3D Camera::getEM() const
{
    // std::cerr << "Making EM:\n";

    // sol.2
    Vector3 forward = this->fromTo;

    Transform3D tr_tilt;
    tr_tilt.rotate(Vector3(0, 0, -this->tilt), false);

    Vector3 up = tr_tilt.getResult(Vector3(0, 1, 0));
    Vector3 right = forward.cross(up);

    up = right.cross(forward).norm();
    right = right.norm();

    Transform3D tr(right, up, forward);
    Vector3 tmp_posi = tr.getResult(this->posi);

    tr.translate(tmp_posi.inverse(), false);

    return tr;
}

Transform3D Camera::getPM(float aspectRatio) const
{
    float tmp_pm[4][4]{};
    float tan_val = tan(toRad(this->Hav));
    float h = this->Hither;
    float y = this->Yon;

    tmp_pm[0][0] = 1;
    tmp_pm[1][1] = aspectRatio;
    tmp_pm[2][2] = (y * tan_val) / (y - h);
    tmp_pm[2][3] = (h * y * tan_val) / (h - y);
    tmp_pm[3][2] = tan_val;

    return Transform3D(tmp_pm);
}