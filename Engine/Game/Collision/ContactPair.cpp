#include "Engine/Game/Collision/ContactPair.h"

#include "Engine/Game/Component/Collider/iCollider.h"
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
		m_left->AddCollisionCount();
		m_right->AddCollisionCount();

		bool isTrigger = false;
		if (m_left->IsTriggerMode())
		{
			m_left->OnTriggerEnter(m_right);
			isTrigger = true;
		}
		if (m_right->IsTriggerMode())
		{
			m_right->OnTriggerEnter(m_left);
			isTrigger = true;
		}
		if (isTrigger)
		{
			return;
		}

		m_left->OnCollisionEnter(m_right, _contactPoint);
		m_right->OnCollisionEnter(m_left, _contactPoint);
	}

	void ContactPair::Stay(const float3& _contactPoint)
	{
		bool isTrigger = false;
		if (m_left->IsTriggerMode())
		{
			m_left->OnTriggerStay(m_right);
			isTrigger = true;
		}
		if (m_right->IsTriggerMode())
		{
			m_right->OnTriggerStay(m_left);
			isTrigger = true;
		}
		if (isTrigger)
		{
			return;
		}

		m_left->OnCollisionStay(m_right, _contactPoint);
		m_right->OnCollisionStay(m_left, _contactPoint);
	}

	void ContactPair::Exit()
	{
		m_left->SubCollisionCount();
		m_right->SubCollisionCount();

		bool isTrigger = false;
		if (m_left->IsTriggerMode())
		{
			m_left->OnTriggerExit(m_right);
			isTrigger = true;
		}
		if (m_right->IsTriggerMode())
		{
			m_right->OnTriggerExit(m_left);
			isTrigger = true;
		}
		if (isTrigger)
		{
			return;
		}

		m_left->OnCollisionExit(m_right);
		m_right->OnCollisionExit(m_left);
	}
}

