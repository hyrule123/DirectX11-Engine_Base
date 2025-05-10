#pragma once
#include "Engine/Game/Component/BaseComponent.h"

namespace core
{
#define BASECOMPONENT_STATEMACHINE BaseComponent<StateMachine, eComponentCategory::BehaviorTree>
    class StateMachine :
        public BASECOMPONENT_STATEMACHINE
    {
        CLASS_INFO(StateMachine, BASECOMPONENT_STATEMACHINE);
    public:
        StateMachine(const std::string_view key);
        virtual ~StateMachine();

        virtual eResult serialize_json(JsonSerializer* _ser) const override { return eResult::Success; }
        virtual eResult deserialize_json(const JsonSerializer* _ser) override { return eResult::Success; }
    };
}


