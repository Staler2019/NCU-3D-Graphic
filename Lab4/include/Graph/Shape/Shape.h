#pragma once

#include "GRGB.h"

enum ShapeType { SHAPE_POINT, SHAPE_LINE, SHAPE_CIRCLE, SHAPE_POLY };

class Shape {
   protected:
    float edge_size;
    GRGB rgb;
    // RGBCode fill_rgbc;
    bool nullObj = false;
    enum ShapeType shape_type;

   public:
    inline Shape(const enum ShapeType shape_type) : shape_type(shape_type) {}
    inline Shape(const enum ShapeType shape_type, const GRGB &rgb, const float edge_size)
        : rgb(rgb), edge_size(edge_size), shape_type(shape_type)
    {
    }

    inline void setRGB(const GRGB &rgb) { this->rgb = rgb; }
    // void setFillRGBC(RGBCode rgbc){
    //     this->fill_rgbc = rgbc;
    //     return;
    // }
    inline void setEdgeSize(const float size) { this->edge_size = size; }
    inline void setNULLObj(const bool val = true) { this->nullObj = val; }

    inline enum ShapeType getShapeType() const { return this->shape_type; }

    inline GRGB getRGB() const { return this->rgb; }

    inline float getEdgeSize() const { return this->edge_size; }
    inline bool isNULLObj() const { return this->nullObj; }

    virtual void draw() const = 0;
    // virtual void fill() = 0;
    // virtual void addTransform() = 0;
};

class Shape3D {};
