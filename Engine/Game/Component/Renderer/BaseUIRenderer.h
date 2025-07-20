#pragma once
#include <Engine/Game/Component/Renderer/StaticMeshRenderer.h>

#include <Engine/Common.h>

namespace core
{
	class BaseUIRenderer :
		public StaticMeshRenderer
	{
		CLASS_INFO(BaseUIRenderer, StaticMeshRenderer);
		REGISTER_FACTORY(StaticMeshRenderer);
		
	public:
		BaseUIRenderer();
		virtual ~BaseUIRenderer();

		//virtual void render() override;

		tCB_CustomData& get_UI_common_data() { return m_UI_data; }

	private:
		tCB_CustomData m_UI_data;
	};
}


