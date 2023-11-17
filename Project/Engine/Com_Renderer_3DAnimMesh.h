#pragma once
#include "Com_Renderer_Mesh.h"

namespace ehw
{
	class Com_Renderer_3DAnimMesh :
		public Com_Renderer_Mesh
	{
	public:
		Com_Renderer_3DAnimMesh();
		virtual ~Com_Renderer_3DAnimMesh();

		virtual void FixedUpdate() override {};
		virtual void Render() override;


	};
}


