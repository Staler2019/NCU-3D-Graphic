#pragma once

#include <algorithm>
#include <iostream>
#include <vector>

#include "GRGB.h"
#include "Math/Vector.h"
#include "Shape/Point.h"
#include "Shape/Shape.h"

enum LineBase { LINE_BASE_X, LINE_BASE_Y, LINE_BASE_Z };

class Line : public Shape {
    Point start_p;
    Point end_p;

   public:
    // start == end will cause error
    inline Line() : Shape(ShapeType::SHAPE_LINE) {}

    inline Line(const Point &start_p, const Point &end_p)
        : start_p(start_p),
          end_p(end_p),
          Shape(ShapeType::SHAPE_LINE)
    {
        // if (isSamePoint(start_p, end_p)) {
        //     std::cerr << "Line constructs with same
        //     point\n"; this->~Line();
        // }
    }
    inline Line(const Vector2 &start_vec2,
                const Vector2 &end_vec2)
        : start_p(Point(start_vec2)),
          end_p(Point(end_vec2)),
          Shape(ShapeType::SHAPE_LINE)
    {
        // if (isSamePoint(start_p, end_p)) {
        //     std::cerr << "Line constructs with same
        //     point\n"; this->~Line();
        // }
    }

    Line(const Point &start_p, const Point &end_p,
         const GRGB &rgb, const float size = 1.0f);

    Line(const Vector2 &start_vec2, const Vector2 &end_vec2,
         const GRGB &rgb, const float size = 1.0f);

    inline Point getStartPoint() const
    {
        return this->start_p;
    }

    inline Point getEndPoint() const { return this->end_p; }

    Point calXCrossPoint(const float x) const;

    Point calYCrossPoint(const float y) const;

    bool isOnLine(const Point &p) const;

    virtual void draw() const override;

    // float getValue(const Point &p) const;
};

struct Line3D : public Shape3D {
   private:
    Vector3 start;
    Vector3 end;

   public:
    inline Line3D() : Shape3D() {}

    inline Line3D(const Vector3 &start, const Vector3 &end)
        : start(start), end(end), Shape3D()
    {
    }

    Vector3 calcZCrossVec(const float z) const;

    Vector3 calcYCrossVec(const float y) const;

    Vector3 calcXCrossVec(const float x) const;

    inline Vector3 getStartVec3() const
    {
        return this->start;
    }

    inline Vector3 getEndVec3() const { return this->end; }

    std::vector<Vector3> getLinePoints() const;

    bool isOnLine(const Vector3 &vec) const;
};