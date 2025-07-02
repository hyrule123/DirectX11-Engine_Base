
#include "StateMachine.h"

namespace core
{
	StateMachine::StateMachine(const std::string_view key)
		: Super(key, s_component_order)
	{
	}
	StateMachine::~StateMachine()
	{
	}
}
