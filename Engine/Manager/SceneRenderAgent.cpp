#include "Engine/Manager/SceneRenderAgent.h"

#include "Engine/Manager/RenderManager.h"

#include "Engine/GPU/StructBuffer.h"
#include "Engine/GPU/ConstBuffer.h"

#include "Engine/Game/Component/Transform.h"
#include "Engine/Game/Component/Light/Light_3D.h"
#include "Engine/Game/Component/Camera/Com_Camera.h"
#include "Engine/Game/Component/Renderer/Renderer.h"
#include "Engine/Game/GameObject.h"

namespace ehw {
	SceneRenderAgent::SceneRenderAgent()
		: m_cameras{}
		, m_mainCamIndex(0u)
		, m_renderers{}
		
	{
	}
	SceneRenderAgent::~SceneRenderAgent()
	{
	}

	void SceneRenderAgent::Init()
	{
		Transform::init_static();
		Light_3D::init_static();
	}

	void SceneRenderAgent::Release()
	{
		m_cameras.clear();
		m_mainCamIndex = 0u;
		m_renderers.clear();
		Light_3D::release_static();
		Transform::release_static();
	}

	void SceneRenderAgent::render()
	{
		for (Com_Camera* cam : m_cameras) {
			if (cam->IsEnabled()) {
				cam->RenderCamera(m_renderers);
			}
		}
	}

	void SceneRenderAgent::frame_end()
	{
		m_renderers.clear();

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

}
