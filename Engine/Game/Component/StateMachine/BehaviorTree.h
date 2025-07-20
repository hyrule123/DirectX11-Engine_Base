#pragma once
#include <Engine/Game/Component/StateMachine/StateMachine.h>

namespace core
{
	class BehaviorTree 
		: public StateMachine
	{
		CLASS_INFO(BehaviorTree, StateMachine);
		
	public:
		BehaviorTree();
		virtual ~BehaviorTree();



		virtual void final_update() override {}
	};
}
