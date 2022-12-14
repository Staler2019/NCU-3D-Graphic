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
    float tilt;  // in rad
    float Hither;  // near
    float Yon;     // far
    float Hav;     // half of FOV // in rad

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

    Transform3D getEM() const;

    Transform3D getPM() const;
};
