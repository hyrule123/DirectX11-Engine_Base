#include "Engine/Manager/SceneRenderAgent.h"

#include "Engine/Manager/RenderManager.h"
#include "Engine/Manager/ResourceManager.h"

#include "Engine/GPU/MultiRenderTarget.h"
#include "Engine/GPU/StructBuffer.h"
#include "Engine/GPU/ConstBuffer.h"

#include "Engine/Game/Component/Transform.h"
#include "Engine/Game/Component/Light/Light_3D.h"
#include "Engine/Game/Component/Camera/Com_Camera.h"
#include "Engine/Game/Component/Renderer/Renderer.h"
#include "Engine/Game/GameObject.h"

#include "Engine/Resource/Mesh/Mesh.h"
#include "Engine/Resource/Material/Material.h"

namespace ehw {
	SceneRenderAgent::SceneRenderAgent()
		: m_cameras{}
		, m_mainCamIndex(0u)
		, m_renderer_queues()
	{
	}
	SceneRenderAgent::~SceneRenderAgent()
	{
	}

	void SceneRenderAgent::Init()
	{
		Transform::init_static();
		Light_3D::init_static();

		m_merge_mesh = ResourceManager<Mesh>::GetInst().Find(strKey::defaultRes::mesh::RectMesh);

		m_merge_material = ResourceManager<Material>::GetInst().Find(strKey::defaultRes::material::MergeMaterial);

		ASSERT(m_merge_material && m_merge_mesh, "merge mesh와 material을 찾지 못했습니다.");
	}

	void SceneRenderAgent::Release()
	{
		m_cameras.clear();
		m_mainCamIndex = 0u;
		for (auto& render_queue : m_renderer_queues) {
			render_queue.clear();
		}
		
		Light_3D::release_static();
		Transform::release_static();
	}

	void SceneRenderAgent::render()
	{
		for (Com_Camera* cam : m_cameras) {
			if (cam->IsEnabled()) {
				//카메라 정보 GPU 바인딩
				cam->bind_data_to_GPU();
				
				//디퍼드 MRT 바인딩
				RenderManager::GetInst().GetMultiRenderTarget(eMRTType::Deffered)->Bind();
				render_by_mode(cam, eRenderingMode::deffered_opaque);
				render_by_mode(cam, eRenderingMode::deffered_mask);

				//// deffered light 
				// 여러개의 모든 빛을 미리 한장의 텍스처에다가 계산을 해두고
				// 붙여버리자
				cam->render_lights();

				//Merge 단계 + Forward Rendering에서는 SwapChain에 바로 데이터를 기록한다.
				RenderManager::GetInst().GetMultiRenderTarget(eMRTType::Swapchain)->Bind();

				// defferd + swapchain merge
				m_merge_material->bind_buffer_to_gpu_register();
				m_merge_mesh->render();

				//Forward Rendering
				render_by_mode(cam, eRenderingMode::forward_opaque);
				render_by_mode(cam, eRenderingMode::forward_cutout);
				render_by_mode(cam, eRenderingMode::forward_transparent);

				//PostProcess 코드는 차후에 수정할것
				//RenderManager::GetInst().CopyRenderTarget();
				//render_by_mode(cam, eRenderingMode::post_process);
			}
		}
	}

	void SceneRenderAgent::frame_end()
	{
		m_renderer_queues.clear();

		//static 버퍼들을 정리해준다.
		Light_3D::clear_buffer_data();
		Transform::clear_buffer_data();
	}

	void SceneRenderAgent::SetResolution(UINT _resX, UINT _resY)
	{
		for (const auto& iter : m_cameras)
		{
			if (iter)
			{
				iter->CreateProjectionMatrix(_resX, _resY);
			}
		}
	}


	void SceneRenderAgent::SetMainCamera(Com_Camera* const _pCam)
	{
		for (size_t i = 0; i < m_cameras.size(); ++i)
		{
			if (_pCam == m_cameras[i])
			{
				m_mainCamIndex = i;
				return;
			}
		}
	}

	Com_Camera* SceneRenderAgent::GetCamera(size_t _Idx) {
		Com_Camera* pCam = nullptr;
		if (m_cameras.size() > (size_t)_Idx)
		{
			pCam = m_cameras[_Idx];
		}

		return pCam;
	}

	void SceneRenderAgent::Register_camera(Com_Camera* const _pCam)
	{
		ASSERT(_pCam, "nullptr"); m_cameras.push_back(_pCam);
	}

	void SceneRenderAgent::Unregister_camera(Com_Camera* const _pCam)
	{
		for (auto iter = m_cameras.begin(); iter != m_cameras.end(); ++iter)
		{
			if (_pCam == (*iter))
			{
				m_cameras.erase(iter);
				return;
			}
		}
	}

	void SceneRenderAgent::enqueue_render(Renderer* _render)
	{
		if (_render && _render->IsRenderReady()) {
			//Entity ID를 활용하여 키를 만들어 준다
			MeshMaterialKey key_maker;
			key_maker.pair.mesh = _render->GetMesh()->GetID();
			key_maker.pair.material = _render->GetCurrentMaterial()->GetID();

			//렌더링 모드별로 정리를 해서 넣어준다.
			int rendering_mode = (int)_render->GetCurrentMaterial()->get_rendering_mode();
			tRenderQueue& myqueue = m_renderer_queues[rendering_mode][key_maker.key];

			if (nullptr == myqueue.mesh) {
				myqueue.mesh = _render->GetMesh();
				myqueue.material = _render->GetCurrentMaterial();
			}

			myqueue.objects_to_render.push_back(_render->gameObject());
		}
	}

	void SceneRenderAgent::render_by_mode(Com_Camera* _cam, eRenderingMode _mode)
	{
		auto iter = m_renderer_queues[(int)_mode].cbegin();
		auto iterEnd = m_renderer_queues[(int)_mode].cend();
		for (; iter != iterEnd; ++iter) {
			_cam->render_gameobjects(iter->second);
		}
	}

}
