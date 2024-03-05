#pragma once
#include "Game/Component/iComponent.h"

namespace ehw
{
	class iCollider
		: public Component<iCollider, eComponentCategory::Collider>
	{
	public:
		iCollider(eColliderType _Type);
		iCollider(const iCollider& _collider) = default;
		virtual ~iCollider();

	private:
		eColliderType m_ComCategory;
	};
}


