#pragma once
#include "Engine/Game/Component/Renderer/Com_Renderer_Mesh.h"

namespace ehw
{
	class Com_Renderer_3DAnimMesh :
		public Com_Renderer_Mesh
	{
		REGISTER_CLASS_INFO(Com_Renderer_3DAnimMesh);
		SET_INSTANCE_ABLE(Com_Renderer_3DAnimMesh);
	public:
		Com_Renderer_3DAnimMesh();
		virtual ~Com_Renderer_3DAnimMesh();


		virtual void Render() override;
	};
}


