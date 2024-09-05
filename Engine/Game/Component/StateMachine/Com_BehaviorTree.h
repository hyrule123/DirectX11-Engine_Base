#pragma once
#include "Engine/Game/Component/StateMachine/StateMachine.h"

namespace ehw
{
	class Com_BehaviorTree 
		: public StateMachine
	{
		CLASS_NAME(Com_BehaviorTree);
		
	public:
		Com_BehaviorTree();
		virtual ~Com_BehaviorTree();



		virtual void final_update() override {}
	};
}
