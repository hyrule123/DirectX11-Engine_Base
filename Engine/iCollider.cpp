#include "PCH_Engine.h"

#include "iCollider.h"

namespace ehw
{
	iCollider::iCollider(eColliderType _Type)
		: iComponent(eComponentCategory::Collider)
		, m_ComCategory(_Type)
	{
	}

	iCollider::~iCollider()
	{
	}
}

