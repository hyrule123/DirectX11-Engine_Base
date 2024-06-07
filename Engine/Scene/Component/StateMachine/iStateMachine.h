#pragma once
#include "Engine/Scene/Component/iComponent.h"

namespace ehw
{
    class iStateMachine :
        public Component <iStateMachine, eComponentCategory::BehaviorTree>
    {
    public:
        iStateMachine();
        virtual ~iStateMachine();

        virtual eResult Serialize_Json(JsonSerializer* _ser) const override { return eResult::Success; }
        virtual eResult DeSerialize_Json(const JsonSerializer* _ser) override { return eResult::Success; }
    };
}

