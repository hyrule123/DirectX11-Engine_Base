#pragma once

#include "GameObject.h"


namespace gui
{
	class DebugObject : public mh::GameObject
	{
	public:
		DebugObject();
		virtual ~DebugObject();

		void FixedUpdate();
	

	private:
	};
}
