#include "Engine/Game/Component/StateMachine/Com_BehaviorTree.h"

namespace ehw
{
    Com_BehaviorTree::Com_BehaviorTree()
        : StateMachine(REGISTER_INSTANCE(Com_BehaviorTree))
    {
    }
    Com_BehaviorTree::~Com_BehaviorTree()
    {
    }
}
