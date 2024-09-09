#pragma once
#include "Engine/Common.h"
#include "Engine/DefaultShader/Light/Light.hlsli"

namespace ehw {
	class Com_Camera;
	class Renderer;
	class Light_3D;
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

		void render();

	public:
		void frame_end();

		void SetResolution(UINT _resX, UINT _resY);

		inline Com_Camera* GetMainCamera() { return GetCamera(m_mainCamIndex); }

		void SetMainCamera(Com_Camera* const _pCam);
		Com_Camera* GetCamera(size_t _Idx);

		void Register_camera(Com_Camera* const _pCam);
		void Unregister_camera(Com_Camera* const _pCam);

		void EnqueueRenderer(Renderer* const _renderer) {
			if (_renderer) { m_renderers.push_back(_renderer); }
		}

	private:
		std::vector<Com_Camera*>			m_cameras;
		size_t								m_mainCamIndex;

		std::vector<Renderer*>				m_renderers;
	};
}

