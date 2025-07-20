
#include "ContactPair.h"

#include <Engine/Game/Component/Collider/Collider.h>
namespace core
{
	ContactPair::ContactPair(const s_ptr<Collider>& _left, const s_ptr<Collider>& _right)
		: m_left(_left)
		, m_right(_right)
	{
	}

	ContactPair::~ContactPair()
	{
	}

	void ContactPair::operator=(const ContactPair& _other)
	{
		m_left = _other.m_left;
		m_right = _other.m_right;
	}

	void ContactPair::Enter(const float3& _contactPoint)
	{
		if (m_left->is_trigger_mode() || m_right->is_trigger_mode())
		{
			m_left->on_trigger_enter(m_right);
			m_right->on_trigger_enter(m_left);
		}
		else
		{
			m_left->on_collision_enter(m_right, _contactPoint);
			m_right->on_collision_enter(m_left, _contactPoint);
		}
	}

	void ContactPair::Stay(const float3& _contactPoint)
	{
		if (m_left->is_trigger_mode() || m_right->is_trigger_mode())
		{
			m_left->on_trigger_stay(m_right);
			m_right->on_trigger_stay(m_left);
		}
		else
		{
			m_left->on_collision_stay(m_right, _contactPoint);
			m_right->on_collision_stay(m_left, _contactPoint);
		}
	}

	void ContactPair::Exit()
	{
		if (m_left->is_trigger_mode() || m_right->is_trigger_mode())
		{
			m_left->on_trigger_exit(m_right);
			m_right->on_trigger_exit(m_left);
		}
		else
		{
			m_left->on_collision_exit(m_right);
			m_right->on_collision_exit(m_left);
		}
	}
}

