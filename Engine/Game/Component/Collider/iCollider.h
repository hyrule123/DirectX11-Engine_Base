#pragma once
#include "../iComponent.h"


namespace ehw
{
	class iCollider
		: public iComponent
	{
	public:
		iCollider(eColliderType _Type);
		iCollider(const iCollider& _collider) = default;
		virtual ~iCollider();

	private:
		eColliderType m_ComCategory;
	};
}


