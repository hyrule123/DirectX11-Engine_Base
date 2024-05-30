#include "Engine/Scene/Collision/ContactPair.h"

#include "Engine/Scene/Component/Collider/iCollider.h"
namespace ehw
{
	ContactPair::ContactPair(iCollider* const _left, iCollider* const _right)
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
		if (m_left->IsTriggerMode() || m_right->IsTriggerMode())
		{
			m_left->OnTriggerEnter(m_right);
			m_right->OnTriggerEnter(m_left);
		}
		else
		{
			m_left->OnCollisionEnter(m_right, _contactPoint);
			m_right->OnCollisionEnter(m_left, _contactPoint);
		}
	}

	void ContactPair::Stay(const float3& _contactPoint)
	{
		if (m_left->IsTriggerMode() || m_right->IsTriggerMode())
		{
			m_left->OnTriggerStay(m_right);
			m_right->OnTriggerStay(m_left);
		}
		else
		{
			m_left->OnCollisionStay(m_right, _contactPoint);
			m_right->OnCollisionStay(m_left, _contactPoint);
		}
	}

	void ContactPair::Exit()
	{
		if (m_left->IsTriggerMode() || m_right->IsTriggerMode())
		{
			m_left->OnTriggerExit(m_right);
			m_right->OnTriggerExit(m_left);
		}
		else
		{
			m_left->OnCollisionExit(m_right);
			m_right->OnCollisionExit(m_left);
		}
	}
}

