#include "Engine/Manager/SceneRenderAgent.h"

#include "Engine/Manager/RenderManager.h"

#include "Engine/GPU/StructBuffer.h"
#include "Engine/GPU/ConstBuffer.h"

#include "Engine/Game/Component/Light/Com_Light3D.h"
#include "Engine/Game/Component/Camera/Com_Camera.h"
#include "Engine/Game/Component/Renderer/Renderer.h"
#include "Engine/Game/GameObject.h"

namespace ehw {
	SceneRenderAgent::SceneRenderAgent()
		: m_cameras{}
		, m_mainCamIndex(0u)
		, m_renderers{}
		, m_lights_3D{}
		, m_lightAttributes{}
		
	{
	}
	SceneRenderAgent::~SceneRenderAgent()
	{
	}

	void SceneRenderAgent::Init()
	{

	}

	void SceneRenderAgent::Release()
	{
		m_cameras.clear();
		m_mainCamIndex = 0u;
		m_renderers.clear();
		m_lightAttributes.clear();
	}

	void SceneRenderAgent::Render()
	{
		for (Com_Camera* cam : m_cameras) {
			if (cam->IsEnabled()) {
				cam->RenderCamera(m_renderers);
			}
		}
	}

	void SceneRenderAgent::FrameEnd()
	{
		m_renderers.clear();
		m_lightAttributes.clear();
		m_lights_3D.clear();
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


	void SceneRenderAgent::RemoveLight(Com_Light3D* const _pComLight)
	{
		for (auto iter = m_lights_3D.begin(); iter != m_lights_3D.end(); ++iter)
		{
			if (_pComLight == (*iter))
			{
				m_lights_3D.erase(iter);
				break;
			}
		}
	}

	void SceneRenderAgent::BindLights()
	{
		StructBuffer* sbuf = RenderManager::GetInst().GetLightSBuffer();

		sbuf->SetData(m_lightAttributes.data(), m_lightAttributes.size());

		eShaderStageFlag_ Flag = eShaderStageFlag::Vertex | eShaderStageFlag::Pixel;

		sbuf->BindDataSRV(GPU::Register::t::lightAttributes, Flag);

		tCB_NumberOfLight trCb = {};
		trCb.numberOfLight = (uint)m_lightAttributes.size();

		//Destroy된 light 포인터 제거
		std::erase_if(m_lights_3D,
			[](Com_Light3D* const iter)->bool
			{
				return iter->IsDestroyed();
			}
		);

		//light index 지정
		for (size_t i = 0; i < m_lights_3D.size(); i++)
		{
			if (m_lights_3D[i]->IsEnabled())
			{
				m_lights_3D[i]->SetIndex((uint)i);
			}
		}

		ConstBuffer* cb = RenderManager::GetInst().GetConstBuffer(eCBType::numberOfLight);
		cb->SetData(&trCb);
		cb->bind_data(Flag);
	}
}
