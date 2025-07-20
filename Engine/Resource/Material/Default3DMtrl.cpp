
#include "Default3DMtrl.h"

#include <Engine/Resource/Shader/GraphicsShader.h>
#include <Engine/GPU/StructBuffer.h>

#ifdef _DEBUG
#include <Engine/CompiledShaderHeader/Deffered3D_VS_x64_Debug.h>
#include <Engine/CompiledShaderHeader/Deffered3D_PS_x64_Debug.h>
#else
#include <Engine/CompiledShaderHeader/Deffered3D_VS_x64_Release.h>
#include <Engine/CompiledShaderHeader/Deffered3D_PS_x64_Release.h>
#endif

#include <Engine/Manager/ResourceManager.h>

#include <Engine/Game/GameObject.h>

#include <Engine/Game/Component/Animator/Animator3D.h>
#include <Engine/Game/Component/Transform.h>
#include <Engine/Game/Component/Animator/Animator3D.h>
#include <Engine/Game/Component/Animator/Animation3D_PlayData.h>

#include <Engine/Resource/Model3D/Skeleton.h>
#include <Engine/Resource/Shader/GraphicsShader.h>

namespace core {
    Default3DMtrl::Default3DMtrl()
        : Material(Default3DMtrl::s_static_type_name)
        , m_default_3D_mtrl_instancing_data()
        , m_default_3D_mtrl_instancing_buffer()
    {
        set_rendering_mode(eRenderingMode::deffered_opaque);
        set_engine_default_res(true);

        m_default_3D_mtrl_instancing_buffer = ResourceManager<StructBuffer>::get_inst().find("default_3D_material_instancing_buffer");
    }
    Default3DMtrl::~Default3DMtrl()
    {
    }

    void Default3DMtrl::clear_instancing_buffer()
    {
        m_default_3D_mtrl_instancing_data.clear();
    }

    void Default3DMtrl::set_data_to_instancing_buffer(const std::vector<s_ptr<GameObject>>& _objs)
    {
        m_default_3D_mtrl_instancing_data.clear();

        if (_objs.empty()) 
        {
            return;
        }

        m_default_3D_mtrl_instancing_data.reserve(_objs.size());

        //애니메이션의 최종 행렬을 먼저 계산 후 바인딩한다.
        s_ptr<Animator3D> animator = _objs.front()->get_component<Animator3D>();
        if (animator)
        {
            animator->bind_computed_final_bone_matrix();
        }

        for (const s_ptr<GameObject>& obj : _objs) {
            s_ptr<Animator3D> animator = obj->get_component<Animator3D>();
            tDefault3DMtrl_InstancingData data{};

            if (animator && animator->is_playing()) {
                data.bAnim = TRUE;
                data.BoneCount = animator->get_shared_play_data()->get_bone_count();
                data.model_instance_ID = animator->get_shared_play_data()->get_model_inst_ID();
            }

            m_default_3D_mtrl_instancing_data.push_back(data);
        }

        m_default_3D_mtrl_instancing_buffer->set_data(m_default_3D_mtrl_instancing_data.data(), (UINT)m_default_3D_mtrl_instancing_data.size());
    }

    void Default3DMtrl::bind_instancing_buffer_to_GPU_register()
    {
        m_default_3D_mtrl_instancing_buffer->bind_buffer_as_SRV();
    }

    void Default3DMtrl::set_forward3D_shader()
    {
        auto forward = ResourceManager<GraphicsShader>::get_inst().find(name::defaultRes::shader::graphics::Forward3DShader);
        set_shader(forward);
    }

    void Default3DMtrl::set_deffered3D_shader()
    {
        auto deffered = ResourceManager<GraphicsShader>::get_inst().find(name::defaultRes::shader::graphics::Deffered3DShader);
        set_shader(deffered);
    }
}
