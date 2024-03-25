#include "Engine/Game/Component/Collider/iCollider.h"

#include "Engine/Game/GameObject.h"
#include "Engine/Manager/CollisionManager.h"

namespace ehw
{
	iCollider::iCollider(eDimensionType _dimension)
		: m_dimension(_dimension)
		, m_isTriggerMode(false)
		, m_isColliding(false)
	{
	}

	iCollider::~iCollider()
	{

	}

	void iCollider::Awake()
	{
		m_transform = GetOwner()->Transform();
	}
	const MATRIX& iCollider::GetWorldMatrix()
	{
		return m_transform->GetWorldMatrix();
	}
}

