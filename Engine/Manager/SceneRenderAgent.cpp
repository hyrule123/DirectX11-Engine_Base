
#include "SceneRenderAgent.h"


#include <Engine/Manager/RenderManager.h>
#include <Engine/Manager/ResourceManager.h>

#include <Engine/GPU/MultiRenderTarget.h>
#include <Engine/GPU/StructBuffer.h>
#include <Engine/GPU/ConstBuffer.h>

#include <Engine/Game/Component/Transform.h>
#include <Engine/Game/Component/Light/Light_3D.h>
#include <Engine/Game/Component/Camera/Camera.h>
#include <Engine/Game/Component/Renderer/Renderer.h>
#include <Engine/Game/GameObject.h>

#include <Engine/Resource/Mesh/Mesh.h>
#include <Engine/Resource/Material/DebugMaterial.h>

namespace core {
	SceneRenderAgent::SceneRenderAgent()
		: m_cameras{}
		, m_main_cam_idx{ 0u }
		, m_renderer_queues()
		, m_debug_meshes_3D{}
		, m_debug_material()
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

	SceneRenderAgent::~SceneRenderAgent()
	{
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
		m_main_cam_idx = 0u;
		
		for (auto& render_queue : m_renderer_queues) {
			render_queue.clear();
		}
	}

	void SceneRenderAgent::render()
	{
		for (w_ptr<Camera> camera : m_cameras) {

			s_ptr<Camera> cam = camera.lock();
			//카메라 정보 GPU 바인딩
			cam->bind_data_to_GPU();
				
			//디퍼드 MRT 바인딩
			RenderManager::get_inst().get_render_target(eMRTType::Deffered)->bind();
			render_by_mode(cam, eRenderingMode::deffered_opaque);
			render_by_mode(cam, eRenderingMode::deffered_mask);

			//디퍼드 MRT를 텍스처로 바인딩한다.

			//// deffered light 
			// 여러개의 모든 빛을 미리 한장의 텍스처에다가 계산을 해두고
			// 붙여버리자
			//렌더타겟을 Light로 변경
			RenderManager::get_inst().get_render_target(eMRTType::Light)->bind();
			for (int i = 0; i < (int)eLightType::END; ++i) {
				cam->render_lights_3D((eLightType)i, m_light_3D_instances[i]);
			}

			//Merge 단계 + Forward Rendering에서는 SwapChain에 바로 데이터를 기록한다.
			RenderManager::get_inst().get_render_target(eMRTType::Swapchain)->bind();

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

	void SceneRenderAgent::set_resolution(UINT _resX, UINT _resY)
	{
		for (const w_ptr<Camera>& camera : m_cameras)
		{
			s_ptr<Camera> iter = camera.lock();
			if (iter)
			{
				iter->CreateProjectionMatrix(_resX, _resY);
			}
		}
	}


	void SceneRenderAgent::set_main_camera(const s_ptr<Camera>& _pCam)
	{
		for (size_t i = 0; i < m_cameras.size(); ++i)
		{
			if (_pCam == m_cameras[i].lock())
			{
				m_main_cam_idx = i;
				return;
			}
		}
	}

	void SceneRenderAgent::register_camera(const s_ptr<Camera>& _pCam)
	{
		ASSERT(_pCam, "nullptr"); m_cameras.push_back(_pCam);
	}

	void SceneRenderAgent::unregister_camera(const s_ptr<Camera>& _pCam)
	{
		for (auto iter = m_cameras.begin(); iter != m_cameras.end(); ++iter)
		{
			if (_pCam == (*iter).lock())
			{
				m_cameras.erase(iter);
				return;
			}
		}
	}

	void SceneRenderAgent::enqueue_render(const s_ptr<Renderer>& _render)
	{
		if (_render && _render->is_render_ready()) {
			//Entity ID를 활용하여 키를 만들어 준다
			tMeshMaterialKey key{};
			key.mesh = _render->get_mesh();
			key.material = _render->get_current_material();

			//렌더링 모드별로 정리를 해서 넣어준다.
			int rendering_mode = (int)_render->get_current_material()->get_rendering_mode();
			tRenderQueue& myqueue = m_renderer_queues[rendering_mode][key];

			if (nullptr == myqueue.mesh) {
				myqueue.mesh = _render->get_mesh();
				myqueue.material = _render->get_current_material();
			}

			myqueue.objects_to_render.push_back(_render->get_owner());
		}
	}


	void SceneRenderAgent::enqueue_light_3D(eLightType _type, const s_ptr<Light3D>& _light)
	{
		if (_light) {
			m_light_3D_instances[(int)_type].push_back(_light);
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
		RenderManager::get_inst().get_render_target(eMRTType::Swapchain)->bind();
		m_debug_material->bind_shader();

		s_ptr<Camera> mainCam = get_main_camera();

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

	void SceneRenderAgent::render_by_mode(const s_ptr<Camera>& _cam, eRenderingMode _mode)
	{
		for (const auto& iter : m_renderer_queues[(int)_mode]){
			_cam->render_gameobjects(iter.second);
		}
	}

	size_t SceneRenderAgent::tMeshMaterialKey::Hasher::operator()(const tMeshMaterialKey& _key) const
	{
		union key_maker {
			static_assert(sizeof(size_t) == sizeof(uint64), "size_t가 64bit가 아님. 처리코드 필요");
			struct key_pair {
				uint32 a, b;
			} pair;
			size_t key;
		} keymaker;
		keymaker.pair.a = _key.mesh->get_instance_ID();
		keymaker.pair.b = _key.material->get_instance_ID();
		return keymaker.key;
	}
}
