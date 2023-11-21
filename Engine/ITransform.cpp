#include "PCH_Engine.h"
#include "ITransform.h"

namespace ehw
{
	ITransform::ITransform()
		: IComponent(define::eComponentType::Transform)
	{
	}
	ITransform::~ITransform()
	{
	}
}
