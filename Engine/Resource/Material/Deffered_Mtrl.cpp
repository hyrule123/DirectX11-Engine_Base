#include "Engine/Resource/Material/Deffered_Mtrl.h"

#include "Engine/Manager/ResourceManager.h"
#include "Engine/Resource/Shader/GraphicsShader.h"

#ifdef _DEBUG
#include "Engine/CompiledShaderHeader/VS_Deffered_x64_Debug.h"
#include "Engine/CompiledShaderHeader/PS_Deffered_x64_Debug.h"
#else
#include "Engine/CompiledShaderHeader/VS_Deffered_x64_Release.h"
#include "Engine/CompiledShaderHeader/PS_Deffered_x64_Release.h"
#endif

#include "Engine/Game/GameObject.h"
#include "Engine/Game/Component/Animator/Com_Animator3D.h"
#include "Engine/Game/Component/Transform.h"

namespace ehw {
    Deffered_Mtrl::Deffered_Mtrl()
        : Material(Deffered_Mtrl::concrete_class_name)
    {
        std::shared_ptr<GraphicsShader> shader = ResourceManager<GraphicsShader>::GetInst().load(strKey::defaultRes::shader::graphics::DefferedShader);

        ASSERT_DEBUG(shader);

        set_shader(shader);

        set_rendering_mode(eRenderingMode::deffered_opaque);
        set_engine_default_res(true);
    }
    Deffered_Mtrl::~Deffered_Mtrl()
    {
    }
    void Deffered_Mtrl::clear_buffers()
    {
		m_transform_data.clear();
    }
	void Deffered_Mtrl::add_data_to_buffer(GameObject* _obj)
	{
		Material::add_data_to_buffer(_obj);

		auto* tr = _obj->GetComponent<Transform>();
		
	}
    void Deffered_Mtrl::upload_buffer_to_GPU(const std::vector<GameObject*>& _objs)
    {
		

		for (GameObject* obj : _objs) {
			auto animator = obj->GetComponent<Com_Animator3D>();

			//애니메이터가 없거나 / 3D 애니메이터가 아니거나 / 재생 중이 아니거나
			//->일반 Mesh 타입으로 렌더링	
			if (nullptr == animator || false == animator->IsPlaying())
			{
				//Com_Renderer_Mesh::render();
				return;
			}

			if (false == IsRenderReady())
			{
				return;
			}


			auto tr = gameObject()->GetComponent<Transform>();
			tr->clear_buffer_data();
			tr->add_to_buffer();
			tr->bind_buffer_to_GPU_register();

			animator->bind_buffer_to_GPU_register();
			//Render
			UINT iSubsetCount = GetMesh()->get_subset_count();
			for (UINT i = 0; i < iSubsetCount; ++i)
			{
				Material* mtrl = GetCurrentMaterial(i);
				if (mtrl)
				{
					//메쉬 바인딩 - Render 내부에서 실행되도록 변경
					//GetMesh()->BindBuffer(i);

					//재질에 애니메이션 정보 넣어주고 바인딩
					mtrl->SetAnim3D(true);
					mtrl->SetBoneCount(GetMesh()->get_skeleton()->GetBoneCount());
					mtrl->bind_buffer_to_gpu_register();

					// 사용할 메쉬 업데이트 및 렌더링
					GetMesh()->render(i);

					//mtrl->unbind_data();
					mtrl->SetBoneCount(0);
					mtrl->SetAnim3D(false);
				}
			}

			animator->unbind_buffer_from_GPU_register();

		}
    }
    void Deffered_Mtrl::bind_buffer_to_gpu_register()
    {
    }
    void Deffered_Mtrl::unbind_buffer_from_gpu_register()
    {
    }
    void Deffered_Mtrl::load_shaders()
    {
        std::shared_ptr<GraphicsShader> deffered_opaque = std::make_unique<GraphicsShader>();

        eResult result = deffered_opaque->compile_from_byte_code(eGSStage::Vertex, VS_Deffered, sizeof(VS_Deffered));
        ASSERT(eResult_success(result), "쉐이더 컴파일 실패");

        result = deffered_opaque->compile_from_byte_code(eGSStage::Pixel, PS_Deffered, sizeof(PS_Deffered));
        ASSERT(eResult_success(result), "쉐이더 컴파일 실패");

        ResourceManager<GraphicsShader>::GetInst().Insert(g_strKey_deffered_opaque_shader, deffered_opaque);
    }
}
