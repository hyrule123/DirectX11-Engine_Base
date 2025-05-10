#pragma once
#include "Engine/Resource/Material/Material.h"

namespace core {
    class Default3DMtrl final :
        public Material 
    {
        CLASS_INFO(Default3DMtrl, Material);
        REGISTER_FACTORY(Default3DMtrl);
        CLONE_ABLE(Default3DMtrl);
    public:
        Default3DMtrl();
        Default3DMtrl(const Default3DMtrl& _other) = default;

        virtual ~Default3DMtrl();

        void clear_instancing_buffer() final;
        void set_data_to_instancing_buffer(const std::vector<GameObject*>& _objs) final;
        void bind_instancing_buffer_to_GPU_register() final;

        void set_forward3D_shader();
        void set_deffered3D_shader();
    private:
        //이 material을 사용하는 각 GameObject가 가지고 있는 정보
        std::vector<tDefault3DMtrl_InstancingData> m_default_3D_mtrl_instancing_data;

        //공용 버퍼(ResourceManager가 보유)
        std::shared_ptr<StructBuffer> m_default_3D_mtrl_instancing_buffer;
    };
}

