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
		CollisionInfo(const std::shared_ptr<iCollider>& _left, const std::shared_ptr<iCollider>& _right);
		CollisionInfo(const CollisionInfo& _other) = default;
		CollisionInfo(CollisionInfo&& _right) noexcept = default;
		~CollisionInfo();

		void operator = (const CollisionInfo& _other);

	private:
		void Enter(const float2& _contactPoint);
		void Stay(const float2& _contactPoint);
		void Exit();

		std::shared_ptr<iCollider> m_left;
		std::shared_ptr<iCollider> m_right;
	};
}

