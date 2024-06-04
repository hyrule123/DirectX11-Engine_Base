#include "Engine/Scene/SceneRenderer.h"

#include "Engine/Scene/GameObject.h"

namespace ehw {
	SceneRenderer::SceneRenderer(iScene* _scene)
		: m_scene(_scene)
	{
		ASSERT(m_scene, "Scene 포인터가 등록되지 않았습니다.");

		Entity::SetStrKey("Scene Renderer");
	}
	SceneRenderer::~SceneRenderer()
	{
	}

	void SceneRenderer::Render()
	{

	}
}
