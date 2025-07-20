#include "Light.h"

namespace core
{
	Light::Light(const std::string_view key, eDimensionType _dimensionType)
		: Super(key, s_component_order)
		, m_dimension(_dimensionType)
	{
	}

	Light::~Light()
	{
	}
}


