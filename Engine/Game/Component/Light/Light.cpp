#include "Engine/Game/Component/Light/Light.h"

namespace ehw
{
	Light::Light(const std::string_view key, eDimensionType _dimensionType)
		: Component(key)
		, mDimension(_dimensionType)
	{
	}

	Light::~Light()
	{
	}
}


