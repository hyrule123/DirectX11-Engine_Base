#pragma once
#include <Engine/Game/Component/Component.h>

namespace core
{
    class StateMachine :
        public Component
    {
        CLASS_INFO(StateMachine, Component);
        BASE_COMPONENT(eComponentOrder::StateMachine);

        NO_SERIALIZE_BINARY;
        NO_SERIALIZE_JSON;

    public:
        StateMachine();
        virtual ~StateMachine();
    };
}


