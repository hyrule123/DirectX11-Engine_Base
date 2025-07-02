#pragma once
#include <Engine/Game/Component/Component.h>

namespace core
{
    class StateMachine :
        public Component
    {
        CLASS_INFO(StateMachine, Component);
        BASE_COMPONENT(eComponentOrder::StateMachine);

    public:
        StateMachine(const std::string_view key);
        virtual ~StateMachine();

        virtual eResult serialize_json(JsonSerializer* _ser) const override { return eResult::Success; }
        virtual eResult deserialize_json(const JsonSerializer* _ser) override { return eResult::Success; }
    };
}


