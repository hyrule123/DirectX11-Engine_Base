#pragma once
#include "Engine/Common.h"

namespace ehw {
	class Com_Camera;
	class Renderer;
	class Com_Light3D;
	class Com_Light;
	class StructBuffer;
	class Texture;

	class SceneRenderAgent
	{
		friend class RenderManager;

	public:
		void Init();
		void Release();
		void FrameEnd();

		void SetResolution(UINT _resX, UINT _resY);

		void BindLights();

		inline Com_Camera* GetMainCamera() { return GetCamera(m_mainCamIndex); }

		void SetMainCamera(Com_Camera* const _pCam);
		inline Com_Camera* GetCamera(size_t _Idx);

		void RegisterCamera(Com_Camera* const _pCam);
		void RemoveCamera(Com_Camera* const _pCam);

		void EraseIfDestroyed_Camera(bool _callRender);

		inline void EnqueueRenderer(Renderer* const _renderer);
		inline const std::vector<Renderer*>& GetRenderers() { return m_renderers; }
		//Renderer
		void PushLightAttribute(const tLightAttribute& lightAttribute) { m_lightAttributes.push_back(lightAttribute); }
		inline void RegisterLight(Com_Light3D* const _pComLight);
		void RemoveLight(Com_Light3D* const _pComLight);

		const std::vector<Com_Light3D*>& GetLights() { return m_lights_3D; }

	private:
		SceneRenderAgent();
		~SceneRenderAgent();

		std::vector<Com_Camera*>			m_cameras;
		size_t							m_mainCamIndex;

		std::vector<Renderer*>			m_renderers;

		//생성자-소멸자에서 여기에 등록하기 떄문에 raw pointer를 사용해야 함
		std::vector<Com_Light3D*>			m_lights_3D;
		std::vector<tLightAttribute>		m_lightAttributes;
		
	};


	inline Com_Camera* SceneRenderAgent::GetCamera(size_t _Idx)
	{
		Com_Camera* pCam = nullptr;
		if (m_cameras.size() > (size_t)_Idx)
		{
			pCam = m_cameras[_Idx];
		}


		return pCam;
	}

	inline void SceneRenderAgent::RegisterLight(Com_Light3D* const _pComLight)
	{
		if (_pComLight)
		{
			m_lights_3D.push_back(_pComLight);
		}
	}

	inline void SceneRenderAgent::EnqueueRenderer(Renderer* const _renderer)
	{
		if (_renderer) { m_renderers.push_back(_renderer); }
	};
}

