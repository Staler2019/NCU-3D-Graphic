#pragma once

#include <map>
#include <queue>
#include <vector>

#include "Camera.h"
#include "GObj.h"
#include "GRGB.h"
#include "Light.h"
#include "Shape/Line.h"
#include "Shape/Poly.h"

struct ViewPort {
    float vxl;
    float vxr;
    float vyb;
    float vyt;
};

class Scene3D {
   private:
    std::vector<GObj *> layers;
    GRGB default_rgb;
    GRGB bg_rgb;
    GRGB ambient_rgb;
    int win_width;
    int win_height;
    ViewPort vp;
    std::map<int, Light> lights;

    inline Vector3 perspectiveDivide(const Vector4 vec4) const
    {
        const float w = vec4.v4;
        return Vector3(vec4.v1 / w, vec4.v2 / w, vec4.v3 / w);
    }

    Transform2D getViewportTransform() const;

    void drawViewport(const Transform2D &tr) const;

    inline float getAspectRatio() const
    {
        return this->win_width / this->win_height;
    }

    std::vector<std::vector<float>> initZBuffer() const;

    std::vector<std::vector<GRGB>> initCBuffer(const Transform2D &tr) const;

    void drawBuffer(const std::vector<std::vector<GRGB>> &c_buffer) const;

    // GRGB calcLight(const std::vector<Vector3 *> &obj_Vs,
    //                const Vector3 &cam_posi, const Transform3D &tr_tm,
    //                const GRGB &obj_grgb, const float Kd, const float Ks,
    //                const int N) const;

    // use points's original location
    GRGB calcLight(Vector3 vec3_point, const Vector3 &normal,
                       Vector3 cam_posi, const Transform3D &tr_tm,
                       const GRGB &obj_grgb, const float Kd, const float Ks,
                       const int N, bool modeRV = true) const;

   public:
    inline Scene3D() { this->default_rgb = GRGB(0.0f, 1.0f, 0.0f); }

    inline void setWindow(const int win_width, const int win_height)
    {
        this->win_width = win_width;
        this->win_height = win_height;
    }

    inline void setViewport(const float vxl, const float vxr, const float vyb,
                            const float vyt)
    {
        this->vp.vxl = vxl;
        this->vp.vxr = vxr;
        this->vp.vyb = vyb;
        this->vp.vyt = vyt;
    }

    inline void addLayer(GObj *ol) { this->layers.push_back(ol); }

    void showLines(const Camera &cam) const;

    void showTextures(const Camera &cam) const;  // TODO. lab4

    void clear();

    inline void setBGRGB(const GRGB &rgb) { this->bg_rgb = rgb; }

    inline void setAmbientRGB(const GRGB &rgb) { this->ambient_rgb = rgb; }

    inline void setLight(const int index, const Light &light)
    {
        this->lights[index] = light;
    }
};

void cutFace(std::vector<Vector4> &vec4s);

float getPlaneC(const int plane_i, const Vector4 *vec4);

Vector4 *calcPlaneVec4(const float plane_i, const Vector4 *last_vec4,
                       const Vector4 *now_vec4, const float last_c,
                       const float now_c);
