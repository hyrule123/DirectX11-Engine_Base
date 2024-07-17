#pragma once
#include "Engine/Scene/Component/Component.h"

namespace ehw
{
    class StateMachine :
        public Component <StateMachine, eComponentCategory::BehaviorTree>
    {
    public:
        StateMachine();
        virtual ~StateMachine();

        virtual eResult Serialize_Json(JsonSerializer* _ser) const override { return eResult::Success; }
        virtual eResult DeSerialize_Json(const JsonSerializer* _ser) override { return eResult::Success; }
    };
}


