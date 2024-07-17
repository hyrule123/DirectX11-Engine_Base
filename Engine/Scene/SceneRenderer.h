#pragma once
#include "Engine/Entity.h"

namespace ehw {
	class Scene;
	class SceneRenderer
		: public Entity
	{
	public:
		SceneRenderer(Scene* _scene);
		virtual ~SceneRenderer();

		void Render();

	private:
		Scene* m_scene;
	};
}

