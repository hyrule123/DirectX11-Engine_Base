#pragma once
#include "Engine/Scene/Component/StateMachine/StateMachine.h"

namespace ehw
{
	class Com_BehaviorTree 
		: public StateMachine
	{
	public:
		Com_BehaviorTree();
		virtual ~Com_BehaviorTree();



		virtual void FinalUpdate() override {}
	};
}
