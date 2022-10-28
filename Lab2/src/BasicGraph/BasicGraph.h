#ifndef __BASICGRAPH_H__
#define __BASICGRAPH_H__

#include "./RGBCode.h"

class BasicGraph {
   protected:
    float edge_size;
    RGBCode edge_rgbc;
    RGBCode fill_rgbc;
    // bool nullObj = false;

   public:
    BasicGraph();
    BasicGraph(RGBCode edge_rgbc, float edge_size);
    inline void setEdgeRGBC(RGBCode rgbc) {
        this->edge_rgbc = rgbc;
        return;
    }
    inline void setFillRGBC(RGBCode rgbc) {
        this->fill_rgbc = rgbc;
        return;
    }
    inline void setEdgeSize(float size) {
        this->edge_size = size;
        return;
    }
    // inline bool isNULLObj() { return this->nullObj; }
    // inline bool setNULLObj(bool val = 1) {
    //     this->nullObj = val;
    //     return;
    // }
    // virtual void draw() = 0;
    // virtual void fill() = 0;
};

#endif  // __BASICGRAPH_H__
