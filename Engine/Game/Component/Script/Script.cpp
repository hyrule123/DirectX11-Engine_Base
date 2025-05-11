#include "Engine/Game/Component/Script/Script.h"

namespace core
{
	Script::Script(const std::string_view key)
		: Component(key, eComponentOrder::Script)
	{
	}

	Script::~Script()
	{
	}
}
