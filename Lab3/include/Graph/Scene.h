#pragma once

// TODO.

#include <map>
#include <queue>
#include <vector>

#include "Camera.h"
#include "GRGB.h"
#include "GObj.h"
#include "Shape/Line.h"
#include "Shape/Poly.h"

struct SceneMask {
    float vxl;
    float vxr;
    float vyb;
    float vyt;
};

/**
 * my scene flow:
 * - for each layer
 *      (1) obj make its own transform
 *      (2) make camera transform
 *      (3) move to screen loc
 *      (4) cut to show size
 *      (5) draw/fill
 *      (6) delete each tmp scene
 */
class Scene {
private:
    std::vector<GObj *> layers;
    GRGB default_rgb;
    int win_width;
    int win_height;
    SceneMask sm;


    /**
     * 2D scene cut
     * @param poly pointer from poly (from Camera::castToNear)
     * @param left
     * @param right
     * @param top
     * @param bottom
     * @return
     */
    Poly *cutPoly(const Poly *poly, const float left = this->win_width * (1 - this->sm.vxl) / 2,
                  const float right = this->win_width * (1 + this->sm.vxr) / 2,
                  const float top = this->win_height * (1 + this->sm.vyt) / 2,
                  const float bottom = this->win_height * (1 - this->sm.vyb) / 2) const;

public:
    inline Scene() { this->default_rgb = GRGB(1.0f, 0.0f, 0.0f); }

    inline void setWindow(const int win_width, const int win_height) {
        this->win_width = win_width;
        this->win_height = win_height;
    }

    inline void setViewport(const float vxl, const float vxr, const float vyb, const float vyt) {
        this->sm = {
                .vxl = vxl,
                .vxr = vxr,
                .vyb = vyb,
                .vyt = vyt,
        };
    }

    inline void addLayer(GObj *ol) { this->layers.push_back(ol); }

    void show(const Camera &cam) const;

    void clear();
};


// class Scene {
//     // std::map<int, BasicGraph *> layer; // TODO. only poly now
//     std::map<int, Poly *> layer;  // 0 as the bottom layer // TODO. use factory classes

//    public:
//     Scene();

//     // void addLayer(BasicGraph *bg); // TODO. only poly now
//     void addLayer(Poly *bg);
//     void show(float scene_left, float scene_right, float scene_bottom, float scene_top, float window_left,
//               float window_right, float window_bottom, float window_top);
//     void clear();
// };

// class Scene3D { //TODO.
//     std::map<int, objl::Loader> layer;

//    public:
//     Scene3D();

//     void addObj(objl::Loader* ol);
//     void setViewPort(float xl, float vxr, float vyb, float vyt);
//     void show(Camera cam);
//     void clear();
// };
