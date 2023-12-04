#pragma once
#include "IComponent.h"

namespace ehw
{
	class Com_BehaviorTree : public IComponent
	{
	public:
		Com_BehaviorTree() : IComponent(eComponentCategory::BehaviorTree){};
		virtual ~Com_BehaviorTree() {};

		virtual void InternalUpdate() override {}
	};
}
