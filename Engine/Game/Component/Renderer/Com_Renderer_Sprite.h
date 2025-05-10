#pragma once
#include "Engine/Game/Component/Renderer/Com_Renderer_Mesh.h"

namespace core
{
	class Com_Animator2D;
	class Com_Renderer_Sprite : public Com_Renderer_Mesh
	{
		CLASS_INFO(Com_Renderer_Sprite, Com_Renderer_Mesh);
		REGISTER_FACTORY(Com_Renderer_Sprite);
		CLONE_ABLE(Com_Renderer_Sprite);
	public:
		Com_Renderer_Sprite();

		Com_Renderer_Sprite(const Com_Renderer_Sprite& _other) = default;

		

		virtual ~Com_Renderer_Sprite();
		
		virtual void init() override;
		virtual void Awake() override;
		//virtual void update() override;
		//virtual void render() override;

	private:
		Com_Animator2D* mAnimator;
	};
}
