#pragma once

#include <cmath>

const double math_pi = 3.14159265358979323846;

inline int gFloor(float f) { return int(std::floor(f + 0.5)); }

inline float toRad(float deg) { return deg * math_pi / 180.0f; }

inline float toDegree(float rad) { return rad * 180.0f / math_pi; }
