#include "Engine/Scene/Component/Collider/iCollider.h"

#include "Engine/Scene/GameObject.h"
#include "Engine/Scene/iScene.h"
#include "Engine/Scene/Collision/CollisionSystem.h"

namespace ehw
{
	iCollider::iCollider(eDimensionType _dimension)
		: m_dimension(_dimension)
		, m_isTriggerMode(false)
		, m_transform(nullptr)
		, m_collisionSystem(nullptr)
		, m_collisionCount(0)
	{
	}

	iCollider::~iCollider()
	{

	}

	void iCollider::Init()
	{
		m_transform = GetOwner()->Transform();
	}

	void iCollider::Awake()
	{
		m_collisionSystem = GetOwner()->GetScene()->GetCollisionSystem();
	}

	void iCollider::OnCollisionEnter(iCollider* _collider, const float3& _contactPoint)
	{
		AddCollisionCount();

		const GameObject::Scripts& scripts = GetOwner()->GetScripts();

		for (size_t i = 0; i < scripts.size(); ++i)
		{
			scripts[i]->OnCollisionEnter(_collider, _contactPoint);
		}
	}

	void iCollider::OnCollisionStay(iCollider* _collider, const float3& _contactPoint)
	{
		const GameObject::Scripts& scripts = GetOwner()->GetScripts();

		for (size_t i = 0; i < scripts.size(); ++i)
		{
			scripts[i]->OnCollisionStay(_collider, _contactPoint);
		}
	}

	void iCollider::OnCollisionExit(iCollider* _collider)
	{
		SubCollisionCount();

		const GameObject::Scripts& scripts = GetOwner()->GetScripts();

		for (size_t i = 0; i < scripts.size(); ++i)
		{
			scripts[i]->OnCollisionExit(_collider);
		}
	}

	void iCollider::OnTriggerEnter(iCollider* _collider)
	{
		AddCollisionCount();

		const GameObject::Scripts& scripts = GetOwner()->GetScripts();

		for (size_t i = 0; i < scripts.size(); ++i)
		{
			scripts[i]->OnTriggerEnter(_collider);
		}
	}

	void iCollider::OnTriggerStay(iCollider* _collider)
	{
		const GameObject::Scripts& scripts = GetOwner()->GetScripts();

		for (size_t i = 0; i < scripts.size(); ++i)
		{
			scripts[i]->OnTriggerStay(_collider);
		}
	}

	void iCollider::OnTriggerExit(iCollider* _collider)
	{
		SubCollisionCount();

		const GameObject::Scripts& scripts = GetOwner()->GetScripts();

		for (size_t i = 0; i < scripts.size(); ++i)
		{
			scripts[i]->OnTriggerExit(_collider);
		}
	}

}

