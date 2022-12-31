#pragma once

#include "GRGB.h"
#include "Math/Vector.h"

struct Light {
    Vector3 posi;
    GRGB rgb;

    inline Light() {}
    inline Light(const Vector3& posi, const GRGB& rgb) : posi(posi), rgb(rgb) {}
};