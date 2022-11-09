#pragma once

#include "RGBCode.h"

enum GraphType { GRAPH_POINT, GRAPH_LINE, GRAPH_CIRCLE, GRAPH_POLY };

class BasicGraph {
   protected:
    float edge_size;
    RGBCode rgbc;
    // RGBCode fill_rgbc;
    // bool nullObj = false;
    enum GraphType graph_type;

   public:
    BasicGraph(enum GraphType graph_type);
    BasicGraph(enum GraphType graph_type, RGBCode rgbc, float edge_size);  // inherit for children

    void setEdgeRGBC(RGBCode rgbc);
    // void setFillRGBC(RGBCode rgbc);
    void setEdgeSize(float size);

    enum GraphType getGraphType();

    RGBCode getRGBCode();

    float getEdgeSize();

    // bool isNULLObj();
    // bool setNULLObj(bool val = 1);

    virtual void draw() = 0;
    // virtual void fill() = 0;
    // virtual void addTransform() = 0;
};
