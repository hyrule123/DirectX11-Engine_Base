#pragma once

#include "Engine/CommonType.h"
#include <memory>

namespace ehw
{
	class iCollider2D;
	class ContactPair2D
	{
		friend class Collision2D;
	public:
		ContactPair2D() = delete;
		ContactPair2D(iCollider2D* const _left, iCollider2D* const _right);
		ContactPair2D(const ContactPair2D& _other) = default;
		ContactPair2D(ContactPair2D&& _right) noexcept = default;
		~ContactPair2D();

		void operator = (const ContactPair2D& _other);

	private:
		void Enter();
		void Stay();
		void Exit();

		iCollider2D* m_left;
		iCollider2D* m_right;
	};
}

