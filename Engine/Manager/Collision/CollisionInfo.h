#pragma once

#include "Engine/CommonType.h"
#include <memory>

namespace ehw
{
	class iCollider;
	class CollisionInfo
	{
		friend class Collision2D;
	public:
		CollisionInfo() = delete;
		CollisionInfo(iCollider* const _left, iCollider* const _right);
		CollisionInfo(const CollisionInfo& _other) = default;
		CollisionInfo(CollisionInfo&& _right) noexcept = default;
		~CollisionInfo();

		void operator = (const CollisionInfo& _other);

	private:
		void Enter(const float2& _contactPoint);
		void Stay(const float2& _contactPoint);
		void Exit();

		iCollider* m_left;
		iCollider* m_right;
	};
}

