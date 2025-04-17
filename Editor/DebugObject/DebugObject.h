#pragma once
#include <Engine/Game/GameObject.h>


namespace core::editor
{
	class DebugObject : public GameObject
	{
	public:
		DebugObject();
		virtual ~DebugObject();

		void final_update();
	

	private:
	};
}
