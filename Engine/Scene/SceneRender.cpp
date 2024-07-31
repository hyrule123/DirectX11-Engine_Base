#include "Engine/Scene/SceneRender.h"

#include "Engine/Scene/GameObject.h"

namespace ehw {
	SceneRender::SceneRender(Scene* _scene)
		: m_scene(_scene)
	{
		ASSERT(m_scene, "Scene 포인터가 등록되지 않았습니다.");

		Entity::SetStrKey("Scene Renderer");
	}
	SceneRender::~SceneRender()
	{
	}

	void SceneRender::Render()
	{

	}
}
