#pragma once
#include <Engine/Game/Component/Renderer/StaticMeshRenderer.h>

namespace core
{
	class Animator2D;
	class SpriteRenderer : public StaticMeshRenderer
	{
		CLASS_INFO(SpriteRenderer, StaticMeshRenderer);
		REGISTER_FACTORY(SpriteRenderer);
		CLONE_ABLE(SpriteRenderer);
	public:
		SpriteRenderer();

		SpriteRenderer(const SpriteRenderer& _other) = default;

		virtual ~SpriteRenderer();
		
		virtual void init() override;
		virtual void awake() override;
		//virtual void update() override;
		//virtual void render() override;

	private:
		w_ptr<Animator2D> m_animator;
	};
}
