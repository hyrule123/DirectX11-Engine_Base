#include "PCH_Engine.h"
#include "ITransform.h"

namespace ehw
{
	ITransform::ITransform()
		: IComponent(eComponentType::Transform)
	{
	}
	ITransform::~ITransform()
	{
	}
}
