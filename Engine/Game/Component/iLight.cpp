#include "../../PCH_Engine.h"
#include "iLight.h"

namespace ehw
{
	iLight::iLight(eDimensionType _dimensionType)
		: iComponent(eComponentCategory::Light)
		, mDimension(_dimensionType)
	{
	}

	iLight::~iLight()
	{
	}
}


