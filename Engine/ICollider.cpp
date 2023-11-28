#include "PCH_Engine.h"

#include "ICollider.h"

namespace ehw
{
	using namespace ehw::define;
	ICollider::ICollider(eColliderType _Type)
		: IComponent(eComponentType::Collider)
		, mType(_Type)
	{
	}

	ICollider::~ICollider()
	{
	}
}
