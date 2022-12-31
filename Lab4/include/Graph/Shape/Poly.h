#pragma once

#include <vector>

#include "GRGB.h"
#include "Math/Vector.h"
#include "Shape/Line.h"
#include "Shape/Point.h"
#include "Shape/Shape.h"
#include "Transform.h"

class Poly : public Shape {
    std::vector<Point> points;
    bool point_adding_lock = false;  // true: adding point is not allowing

   public:
    inline Poly() : Shape(ShapeType::SHAPE_POLY) {}
    inline Poly(const Point &first_p, const GRGB &rgb,
                const float edge_size = 1.0f)
        : Shape(ShapeType::SHAPE_POLY, rgb, edge_size)
    {
        this->points.emplace_back(first_p);
    }
    Poly(const std::vector<Point> &points, const GRGB &rgb,
         const float edge_size = 1.0f)
        : points(points), Shape(ShapeType::SHAPE_POLY, rgb, edge_size)
    {
        this->point_adding_lock = true;
    }

    inline void addPoint(const Point &p, const bool toDraw = 0)
    {
        if (!point_adding_lock) {
            if (toDraw)
                Line(this->points.back(), p, this->rgb, this->edge_size).draw();
            this->points.emplace_back(p);
        }
    }
    inline void end(const bool toDraw = 0)
    {
        if (toDraw)
            Line(this->points.back(), *(this->points.begin()), this->rgb,
                 this->edge_size)
                .draw();
        this->point_adding_lock = true;
        return;
    }
    void addTransform(Transform2D &tr);

    inline Point popLastPoint()
    {
        if (this->points.size() == 0) {
            std::cerr << "Poly runtime error because "
                         "there's no element is poly's "
                         "vector\n";
            exit(0);
        }

        if (this->point_adding_lock) {
            this->point_adding_lock = 0;
            return *(this->points.begin());
        }

        Point tmp = this->points.back();
        this->points.pop_back();
        return tmp;
    }

    inline bool isEnded() const { return this->point_adding_lock; }

    inline std::size_t size() const { return this->points.size(); }

    inline std::vector<Point> getPoints() const { return this->points; }

    virtual void draw() const override;

    void fill() const;
};

class Poly3D : public Shape3D {
   private:
    std::vector<Vector3> vertexes;

    std::vector<std::vector<Vector3>> toTriangles() const;

   public:
    inline Poly3D(const std::vector<Vector3> &poly_vertexes)
        : vertexes(poly_vertexes), Shape3D()
    {
    }

    std::vector<Vector3> getFillVec3s() const;

    inline std::vector<Vector3> getVertexVec3s() const
    {
        return this->vertexes;
    }
};

std::vector<Vector3> getTriangleFillVec3s(
    const std::vector<Vector3> &tri_vec3s);

// bool isInTriangle(const std::vector<Vector2> &tri_vec2s,
//                   const Vector2 &test_vec2);

// Vector3 calcPlanePoint(const std::vector<Vector3> &vertexes, Vector2 xy);
