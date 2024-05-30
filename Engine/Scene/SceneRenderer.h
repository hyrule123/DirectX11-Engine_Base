#pragma once
#include "Engine/Entity.h"

namespace ehw {
	class iScene;
	class SceneRenderer
		: public Entity
	{
	public:
		SceneRenderer(iScene* _scene);
		virtual ~SceneRenderer();

		void Render();

	private:
		iScene* m_scene;
	};
}

