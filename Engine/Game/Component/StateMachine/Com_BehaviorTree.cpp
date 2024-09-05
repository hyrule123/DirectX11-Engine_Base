#include "Engine/Game/Component/StateMachine/Com_BehaviorTree.h"

namespace ehw
{
    Com_BehaviorTree::Com_BehaviorTree()
        : StateMachine(Com_BehaviorTree::concrete_name)
    {
    }
    Com_BehaviorTree::~Com_BehaviorTree()
    {
    }
}
