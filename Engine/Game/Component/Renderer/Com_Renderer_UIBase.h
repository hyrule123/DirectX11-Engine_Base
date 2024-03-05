#pragma once
#include "Game/Component/Renderer/Com_Renderer_Mesh.h"

namespace ehw
{
	class Com_Renderer_UIBase :
		public Com_Renderer_Mesh
	{
	public:
		Com_Renderer_UIBase();
		virtual ~Com_Renderer_UIBase();

		virtual void Render() override;

		tCB_CustomData& GetUIUniformData() { return mUIData; }

	private:
		tCB_CustomData mUIData;
	};
}


