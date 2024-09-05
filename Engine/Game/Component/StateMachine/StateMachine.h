#pragma once
#include "Engine/Game/Component/Component.h"

namespace ehw
{
    class StateMachine :
        public Component <StateMachine, eComponentCategory::BehaviorTree>
    {
        CLASS_NAME(StateMachine);
    public:
        StateMachine(const std::string_view key);
        virtual ~StateMachine();

        virtual eResult Serialize_Json(JsonSerializer* _ser) const override { return eResult::Success; }
        virtual eResult DeSerialize_Json(const JsonSerializer* _ser) override { return eResult::Success; }
    };
}


