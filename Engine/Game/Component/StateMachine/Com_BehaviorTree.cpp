#include "Engine/Game/Component/StateMachine/Com_BehaviorTree.h"

namespace core
{
    Com_BehaviorTree::Com_BehaviorTree()
        : StateMachine(Com_BehaviorTree::s_static_type_name)
    {
    }
    Com_BehaviorTree::~Com_BehaviorTree()
    {
    }
}
