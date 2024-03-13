#pragma once
#include "Engine/Game/Component/StateMachine/iStateMachine.h"

namespace ehw
{
	class Com_BehaviorTree 
		: public iStateMachine
	{
	public:
		Com_BehaviorTree();
		virtual ~Com_BehaviorTree();



		virtual void LateUpdate() override {}
	};
}
