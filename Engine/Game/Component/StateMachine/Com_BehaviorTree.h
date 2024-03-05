#pragma once
#include "Game/Component/StateMachine/iStateMachine.h"

namespace ehw
{
	class Com_BehaviorTree 
		: public iStateMachine
	{
	public:
		Com_BehaviorTree();
		virtual ~Com_BehaviorTree();



		virtual void InternalUpdate() override {}
	};
}
