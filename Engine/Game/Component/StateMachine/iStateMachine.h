#pragma once
#include "../iComponent.h"

namespace ehw
{
    class iStateMachine :
        public Component <iStateMachine, eComponentCategory::BehaviorTree>
    {
    public:
        iStateMachine();
        virtual ~iStateMachine();
    };
}


