#pragma once

#include "Engine/CommonType.h"
#include <memory>

namespace ehw
{
	class iCollider2D;
	class CollisionInfo
	{
		friend class Collision2D;
	public:
		CollisionInfo() = delete;
		CollisionInfo(iCollider2D* const _left, iCollider2D* const _right);
		CollisionInfo(const CollisionInfo& _other) = default;
		CollisionInfo(CollisionInfo&& _right) noexcept = default;
		~CollisionInfo();

		void operator = (const CollisionInfo& _other);

	private:
		void Enter();
		void Stay();
		void Exit();

		iCollider2D* m_left;
		iCollider2D* m_right;
	};
}

