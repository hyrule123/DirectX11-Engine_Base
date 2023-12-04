#pragma once

#include "GameObject.h"


namespace gui
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
