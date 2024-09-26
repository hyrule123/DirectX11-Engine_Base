#pragma once
#include "Engine/Util/StaticSingleton.h"

namespace ehw {
    class DefaultResources :
        public StaticSingleton<DefaultResources>
    {
        friend class StaticSingleton<DefaultResources>;
    private:
        DefaultResources();
        ~DefaultResources();

        void load_default_mesh();
        void load_default_shaders();
        void load_default_textures();
        void load_default_materials();
        void create_default_buffers();
    };
}


