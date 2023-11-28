#pragma once
#include "IComponent.h"

namespace ehw
{
	class Com_BehaviorTree : public IComponent
	{
	public:
		Com_BehaviorTree() : IComponent(eComponentType::BehaviorTree){};
		virtual ~Com_BehaviorTree() {};

		virtual void FixedUpdate() override {}
	};
}