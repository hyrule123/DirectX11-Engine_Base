#pragma once
#include "Engine/Resource/Material/Material.h"

namespace ehw {
    STR_KEY g_strKey_deffered_opaque_shader = "deffered_opaque";

    class Deffered_Opaque final :
        public Material 
    {
        CLASS_NAME(Deffered_Opaque);
        REGISTER_INSTANCE_DEFAULT(Deffered_Opaque);
        //strKey::defaultRes::shader::graphics::DefferedShader
    public:
        Deffered_Opaque();
        virtual ~Deffered_Opaque();

        virtual void clear_buffer() final;
        virtual void upload_buffer_to_gpu(const std::vector<GameObject*>& _objs) final;
        virtual void bind_buffer_to_gpu_register() final;
        virtual void unbind_buffer_from_gpu_register() final;

        static void load_dedicated_graphics_shader();

    private:

    };
}

