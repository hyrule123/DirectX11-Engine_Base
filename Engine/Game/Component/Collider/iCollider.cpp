#include "Engine/Game/Component/Collider/iCollider.h"

namespace ehw
{
	iCollider::iCollider(eColliderType _Type)
		: m_ComCategory(_Type)
	{
	}

	iCollider::~iCollider()
	{
	}
}

