#pragma once

#include "Engine/CommonType.h"
#include <memory>

namespace ehw
{
	class iCollider;
	class ContactPair
	{
	public:
		ContactPair() = delete;
		ContactPair(iCollider* const _left, iCollider* const _right);
		ContactPair(const ContactPair& _other) = default;
		ContactPair(ContactPair&& _right) noexcept = default;
		~ContactPair();

		void operator = (const ContactPair& _other);

		void Enter(const float3& _contactPoint);
		void Stay(const float3& _contactPoint);
		void Exit();

	private:
		iCollider* m_left;
		iCollider* m_right;

	};
}

