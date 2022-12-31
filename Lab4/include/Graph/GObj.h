#pragma once

#include <cstring>
#include <sstream>
#include <string>
#include <vector>

#include "Math/Vector.h"
#include "Transform.h"
#include "Util.h"
#include "GRGB.h"

using V = Vector3;
using VT = Vector3;
using VN = Vector3;

struct Face {
    std::vector<V*> Vs;
    std::vector<VT*> VTs;
    std::vector<VN*> VNs;

    inline Face(const std::vector<V*>& Vs, const std::vector<VT*>& VTs,
                const std::vector<VN*>& VNs)
        : Vs(Vs), VTs(VTs), VNs(VNs)
    {
    }
};

class GObj {
   private:
    std::string file_name;

    bool thread_lock = false;
    bool loaded = false;

    Transform3D tm;

    std::vector<V> Vs;
    std::vector<VT> VTs;
    std::vector<VN> VNs;
    std::vector<Face> faces;

    void load_thread_job();
    void load();
    void clear();

   public:
    GRGB rgb;
    float Kd;
    float Ks;
    int N; // gloss

    inline GObj(const std::string file_name, const Transform3D& tm, const GRGB& grgb, float Kd, float Ks, int N)
        : file_name(file_name), tm(tm), rgb(grgb), Kd(Kd), Ks(Ks), N(N)
    {
        this->load();
    }

    inline bool fail() const { return !this->loaded; }

    inline Transform3D getTM() const { return this->tm; }

    inline std::vector<Face> getFaces() const { return this->faces; }

    inline std::string getFileName() const { return this->file_name; }
};
