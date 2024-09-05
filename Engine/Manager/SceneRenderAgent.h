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
	private:
		SceneRenderAgent();
		~SceneRenderAgent();
		void Init();
		void Release();

		void Render();

	public:
		void frame_end();

		void SetResolution(UINT _resX, UINT _resY);

		void BindLights();

		inline Com_Camera* GetMainCamera() { return GetCamera(m_mainCamIndex); }

		void SetMainCamera(Com_Camera* const _pCam);
		Com_Camera* GetCamera(size_t _Idx);

		void Register_camera(Com_Camera* const _pCam);
		void Unregister_camera(Com_Camera* const _pCam);

		void EnqueueRenderer(Renderer* const _renderer) {
			if (_renderer) { m_renderers.push_back(_renderer); }
		}
		
		//Renderer
		void PushLightAttribute(const tLightAttribute& lightAttribute) { 
			m_lightAttributes.push_back(lightAttribute); 
		}
		void RegisterLight(Com_Light3D* const _pComLight) {
			if (_pComLight) { m_lights_3D.push_back(_pComLight); }
		}
		void RemoveLight(Com_Light3D* const _pComLight);

		const std::vector<Com_Light3D*>& GetLights() { return m_lights_3D; }

	private:
		std::vector<Com_Camera*>			m_cameras;
		size_t							m_mainCamIndex;

		std::vector<Renderer*>			m_renderers;

		//생성자-소멸자에서 여기에 등록하기 떄문에 raw pointer를 사용해야 함
		std::vector<Com_Light3D*>			m_lights_3D;
		std::vector<tLightAttribute>		m_lightAttributes;
	};
}

