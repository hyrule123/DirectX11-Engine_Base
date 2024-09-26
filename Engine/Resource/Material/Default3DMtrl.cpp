#include "Engine/Resource/Material/Default3DMtrl.h"

#include "Engine/Resource/Shader/GraphicsShader.h"
#include "Engine/GPU/StructBuffer.h"

#ifdef _DEBUG
#include "Engine/CompiledShaderHeader/Deffered3D_VS_x64_Debug.h"
#include "Engine/CompiledShaderHeader/Deffered3D_PS_x64_Debug.h"
#else
#include "Engine/CompiledShaderHeader/Deffered3D_VS_x64_Release.h"
#include "Engine/CompiledShaderHeader/Deffered3D_PS_x64_Release.h"
#endif

#include "Engine/Manager/ResourceManager.h"

#include "Engine/Game/GameObject.h"

#include "Engine/Game/Component/Animator/Com_Animator3D.h"
#include "Engine/Game/Component/Transform.h"
#include "Engine/Game/Component/Animator/Com_Animator3D.h"
#include "Engine/Game/Component/Animator/Animation3D_PlayData.h"

#include "Engine/Resource/Model3D/Skeleton.h"
#include "Engine/Resource/Shader/GraphicsShader.h"

namespace ehw {
    Default3DMtrl::Default3DMtrl()
        : Material(Default3DMtrl::concrete_class_name)
        , m_default_3D_mtrl_instancing_data()
        , m_default_3D_mtrl_instancing_buffer()
    {
        set_rendering_mode(eRenderingMode::deffered_opaque);
        set_engine_default_res(true);

        if (nullptr == s_default_3D_mtrl_instancing_buffer) {
            load_static_buffer();
        }
        m_default_3D_mtrl_instancing_buffer = s_default_3D_mtrl_instancing_buffer;
    }
    Default3DMtrl::~Default3DMtrl()
    {
        m_default_3D_mtrl_instancing_buffer = nullptr;
        if (s_default_3D_mtrl_instancing_buffer.use_count() == 1) {
            s_default_3D_mtrl_instancing_buffer = nullptr;
        }
    }

    void Default3DMtrl::clear_instancing_buffer()
    {
        m_default_3D_mtrl_instancing_data.clear();
    }

    void Default3DMtrl::set_data_to_instancing_buffer(const std::vector<GameObject*>& _objs)
    {
        m_default_3D_mtrl_instancing_data.reserve(_objs.size());

        std::shared_ptr<Skeleton> shared_skeleton = nullptr;
        for (GameObject* obj : _objs) {
            auto* animator = obj->GetComponent<Com_Animator3D>();
            tDefault3DMtrl_InstancingData data{};

            if (animator && animator->is_playing()) {
                //아무 스켈레톤이나 주소를 받아놓는다.
                shared_skeleton = animator->get_shared_play_data()->get_skeleton();

                data.bAnim = TRUE;
                data.BoneCount = animator->get_shared_play_data()->get_bone_count();
                data.model_instance_ID = animator->get_shared_play_data()->get_model_inst_ID();
            }

            m_default_3D_mtrl_instancing_data.push_back(data);
        }

        //애니메이션의 최종 행렬을 계산하고 SRV 바인딩
        if (shared_skeleton) {
            shared_skeleton->compute_bone_final_matrix();
            shared_skeleton->bind_bone_final_matrix_SRV();
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

    void Default3DMtrl::load_static_buffer()
    {
        s_default_3D_mtrl_instancing_buffer = std::make_shared<StructBuffer>();
        StructBuffer::Desc desc{};
        desc.eSBufferType = eStructBufferType::READ_ONLY;
        desc.GPU_register_t_SRV = GPU::Register::t::g_default_3D_mtrl_instancing_buffer;
        desc.GPU_register_u_UAV = GPU::Register::u::NONE;
        s_default_3D_mtrl_instancing_buffer->init<tDefault3DMtrl_InstancingData>(desc);
    }
}
