#pragma once
#include "Engine/Game/Component/iComponent.h"

namespace ehw
{
	class iCollider
		: public Component<iCollider, eComponentCategory::Collider>
	{
	public:
		iCollider(eColliderType _type);
		iCollider(const iCollider& _collider) = default;
		virtual ~iCollider();

		eColliderType GetColliderType() const { return m_colliderType; }

	private:
		eColliderType m_colliderType;
	};
}


