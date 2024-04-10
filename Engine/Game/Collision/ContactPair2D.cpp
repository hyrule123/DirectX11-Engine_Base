#include "Engine/Game/Collision/ContactPair2D.h"

#include "Engine/Game/Component/Collider/iCollider2D.h"
namespace ehw
{
	ContactPair2D::ContactPair2D(iCollider2D* const _left, iCollider2D* const _right)
		: m_left(_left)
		, m_right(_right)
	{
	}

	ContactPair2D::~ContactPair2D()
	{
	}

	void ContactPair2D::operator=(const ContactPair2D& _other)
	{
		m_left = _other.m_left;
		m_right = _other.m_right;
	}

	void ContactPair2D::Enter()
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

		m_left->OnCollisionEnter(m_right);
		m_right->OnCollisionEnter(m_left);
	}

	void ContactPair2D::Stay()
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

		m_left->OnCollisionStay(m_right);
		m_right->OnCollisionStay(m_left);
	}

	void ContactPair2D::Exit()
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

