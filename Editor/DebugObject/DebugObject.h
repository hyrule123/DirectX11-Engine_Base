#pragma once
#include <Engine/Scene/GameObject.h>


namespace ehw::editor
{
	class DebugObject : public GameObject
	{
	public:
		DebugObject();
		virtual ~DebugObject();

		void FinalUpdate();
	

	private:
	};
}
