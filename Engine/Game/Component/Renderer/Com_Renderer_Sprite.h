#pragma once
#include "Engine/Game/Component/Renderer/Com_Renderer_Mesh.h"

namespace ehw
{
	class Com_Animator2D;
	class Com_Renderer_Sprite : public Com_Renderer_Mesh
	{
	public:
		Com_Renderer_Sprite();

		Com_Renderer_Sprite(const Com_Renderer_Sprite& _other) = default;

		CLONE_ABLE(Com_Renderer_Sprite);

		virtual ~Com_Renderer_Sprite();
		
		virtual void Init() override;
		virtual void Awake() override;
		//virtual void Update() override;
		virtual void FinalUpdate() override;
		virtual void Render() override;

	private:
		std::shared_ptr<Com_Animator2D> mAnimator;
	};
}
