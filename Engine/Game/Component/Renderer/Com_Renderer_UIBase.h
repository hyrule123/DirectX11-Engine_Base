#pragma once
#include "Engine/Game/Component/Renderer/Com_Renderer_Mesh.h"

#include "Engine/Common.h"

namespace core
{
	class Com_Renderer_UIBase :
		public Com_Renderer_Mesh
	{
		CLASS_INFO(Com_Renderer_UIBase, Com_Renderer_Mesh);
		REGISTER_FACTORY(Com_Renderer_Mesh);
		
	public:
		Com_Renderer_UIBase();
		virtual ~Com_Renderer_UIBase();

		//virtual void render() override;

		tCB_CustomData& GetUIUniformData() { return mUIData; }

	private:
		tCB_CustomData mUIData;
	};
}


