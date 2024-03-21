#include "Engine/Game/Component/Collider/iCollider.h"

#include "Engine/Game/GameObject.h"
#include "Engine/Manager/CollisionManager.h"

namespace ehw
{
	iCollider::iCollider(eDimensionType _dimension)
		: m_dimension(_dimension)
		, m_isTriggerMode(false)
	{
	}

	iCollider::~iCollider()
	{

	}

	void iCollider::Awake()
	{
		m_transform = GetOwner()->Transform();
		ASSERT_DEBUG(m_transform, "Transform 주소를 받아오지 못했습니다.");
	}
}

