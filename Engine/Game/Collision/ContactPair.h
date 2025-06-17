#pragma once

#include "Engine/Common.h"
#include <memory>

namespace core
{
	class Collider;
	class ContactPair
	{
	public:
		ContactPair() = delete;
		ContactPair(const s_ptr<Collider>& _left, const s_ptr<Collider>& _right);
		ContactPair(const ContactPair& _other) = default;
		ContactPair(ContactPair&& _right) noexcept = default;
		~ContactPair();

		void operator = (const ContactPair& _other);

		void Enter(const float3& _contactPoint);
		void Stay(const float3& _contactPoint);
		void Exit();

	private:
		s_ptr<Collider> m_left;
		s_ptr<Collider> m_right;
	};
}

