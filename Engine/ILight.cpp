#include "PCH_Engine.h"
#include "ILight.h"

namespace ehw
{
	ILight::ILight(define::eDimensionType _dimensionType)
		: IComponent(define::eComponentType::Light)
		, mDimension(_dimensionType)
	{
	}

	ILight::~ILight()
	{
	}
}


