#include "PCH_Engine.h"
#include "ITransform.h"

namespace ehw
{
	ITransform::ITransform()
		: IComponent(eComponentCategory::Transform)
	{
	}
	ITransform::~ITransform()
	{
	}
}
