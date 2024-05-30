#pragma once
#include "Engine/Scene/Component/StateMachine/iStateMachine.h"

namespace ehw
{
	class Com_BehaviorTree 
		: public iStateMachine
	{
	public:
		Com_BehaviorTree();
		virtual ~Com_BehaviorTree();



		virtual void FinalUpdate() override {}
	};
}
