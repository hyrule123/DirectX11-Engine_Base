#include "Engine/Game/Component/Collider/iCollider.h"

namespace ehw
{
	iCollider::iCollider(eColliderType _type)
		: m_colliderType(_type)
	{
	}

	iCollider::~iCollider()
	{
	}
}

