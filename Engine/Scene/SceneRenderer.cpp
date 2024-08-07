#include "Engine/Scene/SceneRenderer.h"

#include "Engine/Scene/GameObject.h"

#include "Engine/Manager/RenderManager.h"
#include "Engine/GPU/StructBuffer.h"

namespace ehw {
	SceneRenderer::SceneRenderer(Scene* _scene)
		: m_scene(_scene)
		, m_cameras()
		, m_mainCamIndex(0)
		, m_renderers()
		, m_lights_3D()
		, m_lightAttributes()
	{
		ASSERT(m_scene, "Scene 포인터가 등록되지 않았습니다.");

		Entity::SetStrKey("Scene Renderer");
	}
	SceneRenderer::~SceneRenderer()
	{
	}

	void SceneRenderer::Render()
	{
		for (auto* cam : m_cameras) {
			if (cam) { cam->RenderCamera(this); }
		}
	}

	void SceneRenderer::UpdateProjectionMatrix(UINT _resX, UINT _resY)
	{
		for (const auto& iter : m_cameras)
		{
			iter->CreateProjectionMatrix(_resX, _resY);
		}
	}

	void SceneRenderer::Unregister_camera(Com_Camera* const _cam)
	{
		for (auto iter = m_cameras.begin(); iter != m_cameras.end(); ++iter) {
			if ((*iter) == _cam) {
				m_cameras.erase(iter);

				//만약 main Cam을 삭제했다면 인덱스를 강제로 0번으로 옮긴다.
				size_t curIdx = iter - m_cameras.begin();
				if (curIdx == m_mainCamIndex) {
					m_mainCamIndex = 0;
				}
			}
		}
	}

	void SceneRenderer::SetMainCamera(Com_Camera* const _pCam)
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

	void SceneRenderer::BindLights()
	{
		StructBuffer* sBuffer = RenderManager::GetInst().GetLight3DStructBuffer();

		ASSERT_DEBUG(sBuffer, "Light Struct Buffer가 없음.");

		sBuffer->SetData(m_lightAttributes.data(), m_lightAttributes.size());

		eShaderStageFlag_ Flag = eShaderStageFlag::Vertex | eShaderStageFlag::Pixel;

		sBuffer->BindDataSRV(GPU::Register::t::lightAttributes, Flag);

		tCB_NumberOfLight trCb = {};
		trCb.numberOfLight = (uint)m_lightAttributes.size();

		//light index 지정
		//for (size_t i = 0; i < m_lights_3D.size(); i++)
		//{
		//	if (m_lights_3D[i]->IsEnabled())
		//	{
		//		m_lights_3D[i]->SetIndex((uint)i);
		//	}
		//}

		//const auto& cb = m_constBuffers[(uint)eCBType::numberOfLight];
		//cb->SetData(&trCb);
		//cb->BindData(Flag);
	}
}
