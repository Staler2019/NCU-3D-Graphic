#pragma once

#include <map>

#include "BasicGraph.h"
#include "Line.h"
#include "Point.h"
#include "Poly.h"

class Scene {
    // std::map<int, BasicGraph *> layer; // TODO. only poly now
    std::map<int, Poly *> layer;  // 0 as the bottom layer // TODO. use factory classes

   public:
    Scene();

    // void addLayer(BasicGraph *bg); // TODO. only poly now
    void addLayer(Poly *bg);
    void show(float scene_left, float scene_right, float scene_bottom, float scene_top, float window_left,
              float window_right, float window_bottom, float window_top);
    void clear();
};
