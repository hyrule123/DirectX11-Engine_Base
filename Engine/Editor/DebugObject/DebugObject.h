#pragma once

#include "../../Game/GameObject.h"


namespace editor
{
	class DebugObject : public ehw::GameObject
	{
	public:
		DebugObject();
		virtual ~DebugObject();

		void InternalUpdate();
	

	private:
	};
}
