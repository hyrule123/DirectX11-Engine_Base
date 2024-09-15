#pragma once
#include "Engine/Resource/Material/Material.h"

namespace ehw {
    STR_KEY g_strKey_deffered_opaque_shader = "deffered_opaque";

    class Deffered_Mtrl final :
        public Material 
    {
        CLASS_NAME(Deffered_Mtrl);
        REGISTER_INSTANCE_DEFAULT(Deffered_Mtrl);
        //strKey::defaultRes::shader::graphics::DefferedShader
    public:
        Deffered_Mtrl();
        virtual ~Deffered_Mtrl();

        virtual void clear_buffers() final;
        virtual void add_data_to_buffer(GameObject* _obj) final;
        virtual void bind_buffer_to_gpu_register() final;

        static void load_shaders();
    private:
        
    };
}

