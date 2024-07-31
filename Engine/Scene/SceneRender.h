#pragma once
#include "Engine/Entity.h"

namespace ehw {
	class Scene;
	class SceneRender
		: public Entity
	{
	public:
		SceneRender(Scene* _scene);
		virtual ~SceneRender();

		void Render();

	private:
		Scene* m_scene;
	};
}

