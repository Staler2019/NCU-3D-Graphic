#pragma once

#include <algorithm>
#include <iostream>

#include "GRGB.h"
#include "Math/Vector.h"
#include "Shape/Point.h"
#include "Shape/Shape.h"

class Line : public Shape {
    enum LineBase { LINE_BASE_X, LINE_BASE_Y };

    Point start_p;
    Point end_p;

   public:
    // start == end will cause error
    inline Line() : Shape(ShapeType::SHAPE_LINE) {}

    inline Line(const Point &start_p, const Point &end_p)
        : start_p(start_p), end_p(end_p), Shape(ShapeType::SHAPE_LINE)
    {
        // if (isSamePoint(start_p, end_p)) {
        //     std::cerr << "Line constructs with same point\n";
        //     this->~Line();
        // }
    }
    inline Line(const Vector2 &start_vec2, const Vector2 &end_vec2)
        : start_p(Point(start_vec2)),
          end_p(Point(end_vec2)),
          Shape(ShapeType::SHAPE_LINE)
    {
        // if (isSamePoint(start_p, end_p)) {
        //     std::cerr << "Line constructs with same point\n";
        //     this->~Line();
        // }
    }

    inline Line(const Point &start_p, const Point &end_p, const GRGB &rgb,
                const float size = 1.0f)
        : start_p(start_p),
          end_p(end_p),
          Shape(ShapeType::SHAPE_LINE, rgb, size)
    {
        if (isSamePoint(start_p, end_p)) {
            std::cerr << "Line constructs with same point\n";
            this->~Line();
        }
    }
    inline Line(const Vector2 &start_vec2, const Vector2 &end_vec2,
                const GRGB &rgb, const float size = 1.0f)
        : start_p(Point(start_vec2)),
          end_p(Point(end_vec2)),
          Shape(ShapeType::SHAPE_LINE, rgb, size)
    {
        if (isSamePoint(this->start_p, this->end_p)) {
            std::cerr << "Line constructs with same point\n";
            this->~Line();
        }
    }

    inline Point getStartPoint() const { return this->start_p; }

    inline Point getEndPoint() const { return this->end_p; }

    inline Point calXCrossPoint(const float x) const
    {
        // x the same won't happen here
        // y = mx+b
        float m = (this->start_p.getY() - this->end_p.getY()) /
                  (this->start_p.getX() - this->end_p.getX());
        float delta_x = x - this->end_p.getX();
        float delta_y = m * delta_x;

        return Point(x, delta_y + this->end_p.getY());
    }

    inline Point calYCrossPoint(const float y) const
    {
        // x = my+b
        float m = (this->start_p.getX() - this->end_p.getX()) /
                  (this->start_p.getY() - this->end_p.getY());
        float delta_y = y - this->end_p.getY();
        float delta_x = m * delta_y;

        return Point(delta_x + this->end_p.getX(), y);
    }

    bool isOnLine(const Point &p) const
    {
        float tmp_x[2] = {this->start_p.getX(), this->end_p.getX()};
        float tmp_y[2] = {this->start_p.getY(), this->end_p.getY()};
        std::sort(tmp_x, tmp_x + 2);
        std::sort(tmp_y, tmp_y + 2);

        float x = p.getX();
        float y = p.getY();

        return (x >= tmp_x[0] && x <= tmp_x[1] && y >= tmp_y[0] &&
                y <= tmp_y[1]);
    }

    virtual void draw() const override;
};

// struct Vector3;

struct Line3D : public Shape3D {
   private:
    struct Vector3 start;
    struct Vector3 end;

   public:
    inline Line3D() : Shape3D() {}

    inline Line3D(const struct Vector3 &start, const struct Vector3 &end)
        : start(start), end(end), Shape3D()
    {
    }

    struct Vector3 calcZCrossVec(const float z) const;

    inline struct Vector3 getStartVec3() const { return this->start; }
    inline struct Vector3 getEndVec3() const { return this->end; }
};