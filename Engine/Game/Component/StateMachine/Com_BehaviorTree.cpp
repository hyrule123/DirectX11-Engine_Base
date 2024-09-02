#include "Engine/Game/Component/StateMachine/Com_BehaviorTree.h"

namespace ehw
{
    Com_BehaviorTree::Com_BehaviorTree()
        : StateMachine(INSTANCE_ABLE(Com_BehaviorTree))
    {
    }
    Com_BehaviorTree::~Com_BehaviorTree()
    {
    }
}
