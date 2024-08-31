#include "Engine/Resource/Animation.h"

namespace ehw
{
	Animation::Animation(const std::string_view key, const std::type_info& _typeInfo)
		:Resource(key, _typeInfo)
	{
	}

	Animation::~Animation()
	{
	}
}

