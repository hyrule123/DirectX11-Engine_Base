#include "BehaviorTree.h"

namespace core
{
    BehaviorTree::BehaviorTree()
        : StateMachine(BehaviorTree::s_static_type_name)
    {
    }
    BehaviorTree::~BehaviorTree()
    {
    }
}
