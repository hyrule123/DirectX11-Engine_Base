#pragma once
#include <Engine/Game/GameObject.h>


namespace ehw::editor
{
	class DebugObject : public GameObject
	{
	public:
		DebugObject();
		virtual ~DebugObject();

		void InternalUpdate();
	

	private:
	};
}
