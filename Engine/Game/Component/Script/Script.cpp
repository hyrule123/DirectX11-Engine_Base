
#include "Script.h"

namespace core
{
	Script::Script(const std::string_view key)
		: Component(key, Script::s_component_order)
	{
	}

	Script::~Script()
	{
	}
}
