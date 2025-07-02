
#include "Light.h"

namespace core
{
	Light::Light(const std::string_view key, eDimensionType _dimensionType)
		: Super(key, s_component_order)
		, mDimension(_dimensionType)
	{
	}

	Light::~Light()
	{
	}
}


