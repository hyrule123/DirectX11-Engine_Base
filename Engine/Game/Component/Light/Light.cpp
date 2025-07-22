#include "Light.h"

namespace core
{
	Light::Light(eDimensionType _dimensionType)
		: Super(s_component_order)
		, m_dimension(_dimensionType)
	{
	}

	Light::~Light()
	{
	}
}


