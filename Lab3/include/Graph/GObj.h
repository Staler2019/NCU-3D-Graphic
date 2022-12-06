#pragma once

#include <cstring>

#include "OBJ_Loader.h"
#include "Transform.h"

class GObj : public objl::Loader {
   private:
    std::string file_name;
    bool loading_lock = false;
    bool loading_ok = false;

    void load_thread_job();
    void load();

   public:
    Transform3D tr;

    inline GObj(const std::string file_name, const Transform3D &tr) : file_name(file_name), tr(tr), objl::Loader()
    {
        this->load();
    }

    inline bool fail() const { return !this->loading_ok; }
};
