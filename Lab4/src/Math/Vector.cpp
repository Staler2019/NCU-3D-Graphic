#include "Math/Vector.h"

#include <iostream>

std::ostream& operator<<(std::ostream& os, const Vector2& that)
{
    os << "(" << that.v1 << ", " << that.v2 << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const Vector3& that)
{
    os << "(" << that.v1 << ", " << that.v2 << ", " << that.v3 << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const Vector4& that)
{
    os << "(" << that.v1 << ", " << that.v2 << ", " << that.v3 << ", "
       << that.v4 << ")";
    return os;
}