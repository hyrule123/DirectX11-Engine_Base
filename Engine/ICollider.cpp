#include "PCH_Engine.h"

#include "ICollider.h"

namespace ehw
{
	ICollider::ICollider(eColliderType _Type)
		: IComponent(eComponentCategory::Collider)
		, m_ComCategory(_Type)
	{
	}

	ICollider::~ICollider()
	{
	}
}

