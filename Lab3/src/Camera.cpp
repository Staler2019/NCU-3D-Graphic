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

Transform3D Camera::getPM() const
{
    float tmp_pm[4][4]{};
    float tan_val = tan(this->Hav);
    float h = this->Hither;
    float y = this->Yon;
    float aspectRatio = 1;

    tmp_pm[0][0] = 1;
    tmp_pm[1][1] = aspectRatio;
    tmp_pm[2][2] = (y * tan_val) / (y - h);
    tmp_pm[2][3] = (h * y * tan_val) / (h - y);
    tmp_pm[3][2] = tan_val;

    return Transform3D(tmp_pm);
}