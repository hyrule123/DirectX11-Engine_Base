#pragma once
#include "Engine/Game/Component/StateMachine/StateMachine.h"

namespace core
{
	class Com_BehaviorTree 
		: public StateMachine
	{
		CLASS_INFO(Com_BehaviorTree, StateMachine);
		
	public:
		Com_BehaviorTree();
		virtual ~Com_BehaviorTree();



		virtual void final_update() override {}
	};
}
