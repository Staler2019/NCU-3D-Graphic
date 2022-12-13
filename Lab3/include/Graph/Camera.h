#pragma once

#include <cmath>
#include <iostream>

#include "Math/Util.h"
#include "Math/Vector.h"
#include "Shape/Point.h"
#include "Shape/Line.h"
#include "Shape/Poly.h"
#include "Transform.h"

struct Camera {
    Vector3 posi;
    Vector3 fromTo;
    float tilt;  //
    float Hither;  // near
    float Yon;     // far
    float Hav;     // half of FOV

    inline void setPosi(const Vector3& posi) { this->posi = posi; }
    inline void setFromTo(const Vector3& fromTo, const float tilt)
    {
        this->fromTo = fromTo.norm();
        // this->fromTo = fromTo;
        this->tilt = tilt;
    }
    inline void setVision(const float Hither, const float Yon, const float Hav)
    {
        this->Hither = Hither;
        this->Yon = Yon;
        this->Hav = Hav;
    }

    /**
     * return Poly* to scene to cast (make all point's z between hither & Yon cast to hither
     * @param poly
     * @param default_rgb
     * @return
     */
//    Poly* castToHither(const std::vector<Vector3>& mesh_points, const GRGB& default_rgb) const;

    Transform3D getEM() const;
    /**
     * need to check
     * @param aspectRatio win_width/win_height
     * @return
     */
    Transform3D getPM(float aspectRatio) const;

    // inline float getH() const{
    //     return tan(toRad(this->Hav / 2)) * this->Hither;
    // }

private:
//    inline Point vec3ToHither(const Vector3& vec) const{
//        if (vec.v3 < this->Hither || vec.v3 > this->Yon) {
//            std::cerr << "Vector out of range of Hither~Yon\n";
//            exit(1);
//        }
//
//        // z: near <= vec.v3 <= far
//        float scale = this->Hither / vec.v3;
//
//        return Point(vec.v1 * scale, vec.v2 * scale);
//    }
//
//    inline int checkPosi(const float z) const{
//        if(z < this->Hither) // too close
//            return 0;
//        else if(z > this->Yon) // too far
//            return 2;
//        else // in
//            return 1;
//    }
};
