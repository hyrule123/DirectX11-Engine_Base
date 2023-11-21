#pragma once

#include "IComponent.h"


namespace ehw
{
	class ICollider
		: public IComponent
	{
	public:
		ICollider(define::eColliderType _Type);
		ICollider(const ICollider& _collider) = default;
		virtual ~ICollider();

	private:
		define::eColliderType mType;
	};
}


