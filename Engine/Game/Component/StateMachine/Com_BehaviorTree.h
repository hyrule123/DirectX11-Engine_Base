#pragma once
#include "Engine/Game/Component/StateMachine/StateMachine.h"

namespace ehw
{
	class Com_BehaviorTree 
		: public StateMachine
	{
		REGISTER_CLASS_INFO(Com_BehaviorTree);
		SET_INSTANCE_ABLE(Com_BehaviorTree);
	public:
		Com_BehaviorTree();
		virtual ~Com_BehaviorTree();



		virtual void FinalUpdate() override {}
	};
}
