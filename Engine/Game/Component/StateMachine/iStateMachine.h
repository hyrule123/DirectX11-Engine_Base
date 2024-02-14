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

        virtual eResult Serialize(JsonSerializer& _ser) override { return eResult::Success; }
        virtual eResult DeSerialize(const JsonSerializer& _ser) override { return eResult::Success; }
    };
}


