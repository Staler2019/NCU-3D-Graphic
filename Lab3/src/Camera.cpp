#include "Camera.h"

#include <iostream>
#include <vector>

#include "Math/Vector.h"
#include "Shape/Point.h"
#include "Shape/Poly.h"

Poly* Camera::castToHither(const std::vector<Vector3>& mesh_points, const GRGB& default_rgb) const{
    std::cerr << "---for each graph to near---\n";
    std::vector<Vector3> vec_points = mesh_points;
    // Vector3's
    // make all lines before Far plane
    Vector3* last_vec = &vec_points[vec_points.size() - 1];
    int last_posi = this->checkPosi(last_vec->v3);
    std::vector<Vector3> tmp_vec_points;

    for (int i = 0; i < vec_points.size(); i++) {
        Vector3* now_vec = &vec_points[i];

        int now_posi = this->checkPosi(now_vec->v3);

        switch(last_posi){
            case 0:{
                switch(now_posi){
                    // no case 0
                    case 1:{
                        tmp_vec_points.push_back(Line3D(*last_vec, *now_vec).calZCrossVec(this->Hither));
                        tmp_vec_points.push_back(*now_vec);
                        break;
                    }
                    case 2:{
                        tmp_vec_points.push_back(Line3D(*last_vec, *now_vec).calZCrossVec(this->Hither));
                        tmp_vec_points.push_back(Line3D(*last_vec, *now_vec).calZCrossVec(this->Yon));
                        break;
                    }
                }
                break;
            }
            case 1:{
                switch(now_posi){
                    case 0:{
                        tmp_vec_points.push_back(Line3D(*last_vec, *now_vec).calZCrossVec(this->Hither));
                        break;
                    }
                    case 1:{
                        tmp_vec_points.push_back(*now_vec);
                        break;
                    }
                    case 2:{
                        tmp_vec_points.push_back(Line3D(*last_vec, *now_vec).calZCrossVec(this->Yon));
                        break;
                    }
                }
                break;
            }
            case 2:{
                switch(now_posi){
                    case 0:{
                        tmp_vec_points.push_back(Line3D(*last_vec, *now_vec).calZCrossVec(this->Yon));
                        tmp_vec_points.push_back(Line3D(*last_vec, *now_vec).calZCrossVec(this->Hither));
                        break;
                    }
                    case 1:{
                        tmp_vec_points.push_back(Line3D(*last_vec, *now_vec).calZCrossVec(this->Yon));
                        tmp_vec_points.push_back(*now_vec);
                        break;
                    }
                    // no case 2
                }
                break;
            }
        }

        last_posi = now_posi;
        last_p = now_p;
    }

    vec_points = tmp_vec_points;

    if (!vec_points.size()) return nullptr;
    for (auto& v : vec_points) std::cerr << v << ", ";
    std::cerr << "\n";

    std::vector<Point> poly_points;

    for(auto& vec:vec_points)
        poly_points.push_back(this->vec3ToHither(vec));

    return new Poly(poly_points, default_rgb, 1.0f);
}