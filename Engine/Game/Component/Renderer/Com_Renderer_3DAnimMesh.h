#pragma once
#include "Engine/Game/Component/Renderer/Com_Renderer_Mesh.h"

namespace core
{
	class Com_Renderer_3DAnimMesh :
		public Com_Renderer_Mesh
	{
		CLASS_NAME(Com_Renderer_3DAnimMesh);
		REGISTER_INSTANCE_DEFAULT(Com_Renderer_3DAnimMesh);
		
	public:
		Com_Renderer_3DAnimMesh();
		virtual ~Com_Renderer_3DAnimMesh();


		//virtual void render() override;
	};
}


