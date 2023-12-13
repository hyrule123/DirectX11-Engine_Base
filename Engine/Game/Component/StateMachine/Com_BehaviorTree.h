#pragma once
#include "../iComponent.h"

namespace ehw
{
	class Com_BehaviorTree : public iComponent
	{
	public:
		Com_BehaviorTree() : iComponent(eComponentCategory::BehaviorTree){};
		virtual ~Com_BehaviorTree() {};

		virtual void InternalUpdate() override {}
	};
}
