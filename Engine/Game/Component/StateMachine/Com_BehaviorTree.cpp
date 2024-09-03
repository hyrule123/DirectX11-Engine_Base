#include "Engine/Game/Component/StateMachine/Com_BehaviorTree.h"

namespace ehw
{
    Com_BehaviorTree::Com_BehaviorTree()
        : StateMachine(ClassInfo<Com_BehaviorTree>::name())
    {
    }
    Com_BehaviorTree::~Com_BehaviorTree()
    {
    }
}
