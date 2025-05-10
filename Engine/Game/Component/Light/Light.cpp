#include "Engine/Game/Component/Light/Light.h"

namespace core
{
	Light::Light(const std::string_view key, eDimensionType _dimensionType)
		: Super(key)
		, mDimension(_dimensionType)
	{
	}

	Light::~Light()
	{
	}
}


