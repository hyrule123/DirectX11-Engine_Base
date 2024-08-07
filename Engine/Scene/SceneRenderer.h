#pragma once
#include "Engine/Entity.h"

namespace ehw {
	class Scene;
	class Com_Camera;
	class Renderer;
	class Com_Light3D;
	class StructBuffer;

	class SceneRenderer
		: public Entity
	{
	public:
		SceneRenderer(Scene* _scene);
		virtual ~SceneRenderer();

		void Render();
		void BindLights();

		void UpdateProjectionMatrix(UINT _resX, UINT _resY);

		inline void Register_camera(Com_Camera* const _cam) {
			if (_cam) { m_cameras.push_back(_cam); }
		}
		void Unregister_camera(Com_Camera* const _cam);
		Com_Camera* Get_main_camera() {
			return m_cameras[m_mainCamIndex];
		}
		const std::vector<Com_Camera*>& GetCameras() { return m_cameras; }

		inline void EnqueueRenderer(Renderer* _renderer) {
			if (_renderer) { m_renderers.push_back(_renderer); }
		}
		inline void EnqueueLight3D(Com_Light3D* _light) {
			if (_light) { m_lights_3D.push_back(_light); }
		}

		void SetMainCamera(Com_Camera* const _cam);

		std::vector<Com_Light3D*> const& GetLights3D() { return m_lights_3D; };

	private:
		Scene* m_scene;

		std::vector<Com_Camera*>		m_cameras;
		size_t							m_mainCamIndex;
		std::vector<Renderer*>			m_renderers;

		std::vector<Com_Light3D*>			m_lights_3D;
		std::vector<tLightAttribute>		m_lightAttributes;
	};
}

