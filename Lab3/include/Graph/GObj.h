#pragma once

#include <cstring>
#include <vector>

#include "Math/Vector.h"
#include "Transform.h"

struct Face {
    std::vector<Vector3> vertexes;
    inline Face(std::vector<Vector3> vertexes) : vertexes(vertexes) {}
};

class GObj {
   private:
    std::string file_name;
    bool thread_lock = false;
    bool loaded = false;
    Transform3D tm;
    std::vector<Vector3> loaded_vertexes;
    std::vector<Face> loaded_faces;

    void load_thread_job();
    void load();

   public:
    inline GObj(const std::string file_name, const Transform3D& tm)
        : file_name(file_name), tm(tm)
    {
        this->load();
    }

    inline bool fail() const { return !this->loaded; }

    inline Transform3D getTM() const { return this->tm; }

    inline std::vector<Face> getLoadedFaces() const
    {
        return this->loaded_faces;
    }

    inline std::string getFileName() const { return this->file_name; }
};
