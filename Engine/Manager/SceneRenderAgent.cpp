
#include "SceneRenderAgent.h"


#include <Engine/Manager/RenderManager.h>
#include <Engine/Manager/ResourceManager.h>

#include <Engine/GPU/MultiRenderTarget.h>
#include <Engine/GPU/StructBuffer.h>
#include <Engine/GPU/ConstBuffer.h>

#include <Engine/Game/Component/Transform.h>
#include <Engine/Game/Component/Light/Light_3D.h>
#include <Engine/Game/Component/Camera/Com_Camera.h>
#include <Engine/Game/Component/Renderer/Renderer.h>
#include <Engine/Game/GameObject.h>

#include <Engine/Resource/Mesh/Mesh.h>
#include <Engine/Resource/Material/DebugMaterial.h>

namespace core {
	SceneRenderAgent::SceneRenderAgent()
		: m_cameras{}
		, m_mainCamIndex(0u)
		, m_renderer_queues()
		, m_debug_meshes_3D{}
		, m_debug_material()
	{
	}
	SceneRenderAgent::~SceneRenderAgent()
	{
	}

	void SceneRenderAgent::init()
	{
		m_deffered_merge_rectmesh = ResourceManager<Mesh>::get_inst().find(name::defaultRes::mesh::RectMesh);
		m_deffered_merge_material = ResourceManager<Material>::get_inst().find(name::defaultRes::material::MergeMaterial);

		ASSERT(m_deffered_merge_material && m_deffered_merge_rectmesh, "merge mesh와 material을 찾지 못했습니다.");

		m_debug_meshes_3D[(int)eCollider3D_Shape::Cube] =
			ResourceManager<Mesh>::get_inst().find(name::defaultRes::mesh::DebugCubeMesh);

		m_debug_material = ResourceManager<Material>::get_inst().find<DebugMaterial>(name::defaultRes::material::DebugMaterial);
	}

	void SceneRenderAgent::release()
	{
		m_deffered_merge_rectmesh = nullptr;
		m_deffered_merge_material = nullptr;
		for (auto& mesh : m_debug_meshes_3D) {
			mesh = nullptr;
		}
		m_debug_material = nullptr;

		m_cameras.clear();
		m_mainCamIndex = 0u;
		
		for (auto& render_queue : m_renderer_queues) {
			render_queue.clear();
		}
	}

	void SceneRenderAgent::render()
	{
		for (Com_Camera* cam : m_cameras) {
			//카메라 정보 GPU 바인딩
			cam->bind_data_to_GPU();
				
			//디퍼드 MRT 바인딩
			RenderManager::get_inst().get_rendertarget(eMRTType::Deffered)->Bind();
			render_by_mode(cam, eRenderingMode::deffered_opaque);
			render_by_mode(cam, eRenderingMode::deffered_mask);

			//디퍼드 MRT를 텍스처로 바인딩한다.

			//// deffered light 
			// 여러개의 모든 빛을 미리 한장의 텍스처에다가 계산을 해두고
			// 붙여버리자
			//렌더타겟을 Light로 변경
			RenderManager::get_inst().get_rendertarget(eMRTType::Light)->Bind();
			for (int i = 0; i < (int)eLightType::END; ++i) {
				cam->render_lights_3D((eLightType)i, m_light_3D_instances[i]);
			}

			//Merge 단계 + Forward Rendering에서는 SwapChain에 바로 데이터를 기록한다.
			RenderManager::get_inst().get_rendertarget(eMRTType::Swapchain)->Bind();

			// defferd + swapchain merge
			m_deffered_merge_material->bind_shader();
			m_deffered_merge_rectmesh->render();

			//Forward Rendering
			render_by_mode(cam, eRenderingMode::forward_opaque);
			render_by_mode(cam, eRenderingMode::forward_cutout);
			render_by_mode(cam, eRenderingMode::forward_transparent);

			//PostProcess 코드는 차후에 수정할것
			//RenderManager::get_inst().copy_rendertarget();
			//render_by_mode(cam, eRenderingMode::post_process);
		}
	}

	void SceneRenderAgent::frame_end()
	{
		for (auto& render_queue : m_renderer_queues) {
			render_queue.clear();
		}

		for (int i = 0; i < LIGHT_TYPE_MAX; ++i) {
			m_light_3D_instances[i].clear();
		}

		for (auto& vec : m_debug_draw_data_3D) {
			vec.clear();
		}
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
			MeshMaterialKey key{};
			key.mesh = _render->GetMesh();
			key.material = _render->GetCurrentMaterial();

			//렌더링 모드별로 정리를 해서 넣어준다.
			int rendering_mode = (int)_render->GetCurrentMaterial()->get_rendering_mode();
			tRenderQueue& myqueue = m_renderer_queues[rendering_mode][key];

			if (nullptr == myqueue.mesh) {
				myqueue.mesh = _render->GetMesh();
				myqueue.material = _render->GetCurrentMaterial();
			}

			myqueue.objects_to_render.push_back(_render->get_owner());
		}
	}


	void SceneRenderAgent::set_debug_render(bool _is_enable)
	{
		if (_is_enable) {
			m_debug_material =
				ResourceManager<Material>::get_inst().find<DebugMaterial>(name::defaultRes::material::DebugMaterial);
		}
		else {
			m_debug_material = nullptr;
		}
	}

	void SceneRenderAgent::render_debug()
	{
		RenderManager::get_inst().get_rendertarget(eMRTType::Swapchain)->Bind();
		m_debug_material->bind_shader();

		Com_Camera* mainCam = GetMainCamera();

		if (mainCam)
		{
			MATRIX VP = mainCam->GetViewMatrix() * mainCam->GetProjectionMatrix();
			for (int i = 0; i < (int)eCollider3D_Shape::END; ++i) {
				if (m_debug_material && m_debug_meshes_3D[i]) {

					//현재 W까지만 들어있으므로 VP를 곱해서 WVP를 만들어준다.
					for (tDebugDrawData& data : m_debug_draw_data_3D[i]) {
						data.WVP *= VP;
					}

					m_debug_material->set_data_and_bind_GPU(m_debug_draw_data_3D[i]);
					m_debug_meshes_3D[i]->render_instanced((UINT)m_debug_draw_data_3D[i].size());
				}
			}
		}
	}

	void SceneRenderAgent::render_by_mode(Com_Camera* _cam, eRenderingMode _mode)
	{
		for (const auto& iter : m_renderer_queues[(int)_mode]){
			_cam->render_gameobjects(iter.second);
		}
	}

	size_t SceneRenderAgent::MeshMaterialKey::Hasher::operator()(const MeshMaterialKey& _key) const
	{
		union key_maker {
			static_assert(sizeof(size_t) == sizeof(uint64), "size_t가 64bit가 아님. 처리코드 필요");
			struct key_pair {
				uint32 a, b;
			} pair;
			size_t key;
		} keymaker;
		keymaker.pair.a = _key.mesh->GetID();
		keymaker.pair.b = _key.material->GetID();
		return keymaker.key;
	}
}
