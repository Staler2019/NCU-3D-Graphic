#pragma once

#include <cmath>
#include <iostream>

#include "Math/Util.h"
#include "Math/Vector.h"
#include "Shape/Point.h"
#include "Shape/Line.h"
#include "Shape/Poly.h"

struct Camera {
    Vector3 posi;
    Vector3 fromTo;
    float tilt;
    float Hither;  // near
    float Yon;     // far
    float Hav;     // FOV

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

//    inline Vector3 getPosi() const { return this->posi; }
//    inline Vector3 getFromTo() const { return this->fromTo; }
//
//    inline float getTilt() const { return this->tilt; }
//    inline float getNear() const { return this->Hither; }
//    inline float getFar() const { return this->Yon; }
//    inline float getFOV() const { return this->Hav; }

    /**
     * return Poly* to scene to cast (make all point's z between hither & Yon cast to hither
     * @param poly
     * @param default_rgb
     * @return
     */
    Poly* castToHither(const std::vector<Vector3>& mesh_points, const GRGB& default_rgb) const;
private:
    inline Vector3 vec3ToHither(const Vector3& vec) const{
        if (vec.v3 < this->Hither || vec.v3 > this->Yon) {
            std::cerr << "Vector out of range of Hither~Yon\n";
            exit(1);
        }

        // z: near <= vec.v3 <= far
        float scale = this->Hither / vec.v3;

        return Vector3(vec.v1 * scale, vec.v2 * scale, this->Hither);
    }
//    inline Point castToNear(const Vector3& vec) const
//    {
//        if (vec.v3 < this->Hither || vec.v3 > this->Yon) { // TODO. if a line from outside to inside
//            std::cerr << "Vector out of near/far\n";
//            Point tmp;
//            tmp.setNULLObj();
//            return tmp;
//        }
//        else {  // z: near <= vec.v3 <= far
//            float scale = this->Hither / vec.v3;
//            return Point(vec.v1 * scale, vec.v2 * scale);
//        }
//    }
};
