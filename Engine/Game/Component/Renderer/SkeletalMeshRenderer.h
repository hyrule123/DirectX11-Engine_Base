#pragma once
#include <Engine/Game/Component/Renderer/StaticMeshRenderer.h>

namespace core
{
	class SkeletalMeshRenderer :
		public StaticMeshRenderer
	{
		CLASS_INFO(SkeletalMeshRenderer, StaticMeshRenderer);
		REGISTER_FACTORY(SkeletalMeshRenderer);
		
	public:
		SkeletalMeshRenderer();
		virtual ~SkeletalMeshRenderer();


		//virtual void render() override;
	};
}


