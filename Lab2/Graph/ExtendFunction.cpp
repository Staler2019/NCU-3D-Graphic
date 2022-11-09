#include "ExtendFunction.h"

#include <cmath>

#include "Point.h"

int myFloor(float f) { return int(std::floor(f + 0.5)); }

bool isSamePoint(Point p1, Point p2) { return (p1.getX() == p2.getX() && p1.getY() == p2.getY()); }
