#include "PCH_Engine.h"
#include "ILight.h"

namespace ehw
{
	ILight::ILight(eDimensionType _dimensionType)
		: IComponent(eComponentType::Light)
		, mDimension(_dimensionType)
	{
	}

	ILight::~ILight()
	{
	}
}


