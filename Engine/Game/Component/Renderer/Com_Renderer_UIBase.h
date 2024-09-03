#pragma once
#include "Engine/Game/Component/Renderer/Com_Renderer_Mesh.h"

#include "Engine/Common.h"

namespace ehw
{
	class Com_Renderer_UIBase :
		public Com_Renderer_Mesh
	{
		REGISTER_CLASS_INSTANCE_ABLE(Com_Renderer_UIBase);
		
	public:
		Com_Renderer_UIBase();
		virtual ~Com_Renderer_UIBase();

		virtual void Render() override;

		tCB_CustomData& GetUIUniformData() { return mUIData; }

	private:
		tCB_CustomData mUIData;
	};
}


