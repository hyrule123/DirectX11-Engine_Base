#pragma once
#include "Engine/Resource/Material/Material.h"

namespace ehw {
    class Default3DMtrl final :
        public Material 
    {
        CLASS_NAME(Default3DMtrl);
        REGISTER_INSTANCE_DEFAULT(Default3DMtrl);
        //strKey::defaultRes::shader::graphics::Deffered3DShader
    public:
        Default3DMtrl();
        virtual ~Default3DMtrl();

        void clear_instancing_buffer() final;
        void set_data_to_instancing_buffer(const std::vector<GameObject*>& _objs) final;
        void bind_instancing_buffer_to_GPU_register() final;

        static void load_static_buffer();
    private:
        //이 material을 사용하는 각 GameObject가 가지고 있는 정보
        std::vector<tUniqueMtrlData_Default3D> m_default_3D_mtrl_instancing_data;
        std::shared_ptr<StructBuffer> m_default_3D_mtrl_instancing_buffer;

        //동일 material 클래스끼리는 공유해서 사용하므로 static
        static inline std::shared_ptr<StructBuffer> s_default_3D_mtrl_instancing_buffer = nullptr;
    };
}

