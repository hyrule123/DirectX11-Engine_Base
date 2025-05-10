#pragma once
#include "Engine/Game/Component/Renderer/Com_Renderer_Mesh.h"

namespace core
{
	class Com_Renderer_3DAnimMesh :
		public Com_Renderer_Mesh
	{
		CLASS_INFO(Com_Renderer_3DAnimMesh, Com_Renderer_Mesh);
		REGISTER_FACTORY(Com_Renderer_3DAnimMesh);
		
	public:
		Com_Renderer_3DAnimMesh();
		virtual ~Com_Renderer_3DAnimMesh();


		//virtual void render() override;
	};
}


