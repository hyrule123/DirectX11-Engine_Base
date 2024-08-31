#include "Engine/Game/Component/Collider/Collider.h"

#include "Engine/Game/GameObject.h"
#include "Engine/Game/Scene.h"
#include "Engine/Game/Collision/CollisionSystem.h"

namespace ehw
{
	Collider::Collider(const std::string_view key, eDimensionType _dimension)
		: Component(key)
		, m_dimension(_dimension)
		, m_isTriggerMode(false)
		, m_transform(nullptr)
		, m_collisionSystem(nullptr)
		, m_collisionCount(0)
	{
	}

	Collider::~Collider()
	{

	}

	void Collider::Init()
	{
		m_transform = gameObject()->Transform();
	}

	void Collider::Awake()
	{
		m_collisionSystem = gameObject()->scene()->GetCollisionSystem();
	}

	void Collider::OnCollisionEnter(Collider* _collider, const float3& _contactPoint)
	{
		AddCollisionCount();

		const GameObject::Scripts& scripts = gameObject()->GetScripts();

		for (size_t i = 0; i < scripts.size(); ++i)
		{
			scripts[i]->OnCollisionEnter(_collider, _contactPoint);
		}
	}

	void Collider::OnCollisionStay(Collider* _collider, const float3& _contactPoint)
	{
		const GameObject::Scripts& scripts = gameObject()->GetScripts();

		for (size_t i = 0; i < scripts.size(); ++i)
		{
			scripts[i]->OnCollisionStay(_collider, _contactPoint);
		}
	}

	void Collider::OnCollisionExit(Collider* _collider)
	{
		SubCollisionCount();

		const GameObject::Scripts& scripts = gameObject()->GetScripts();

		for (size_t i = 0; i < scripts.size(); ++i)
		{
			scripts[i]->OnCollisionExit(_collider);
		}
	}

	void Collider::OnTriggerEnter(Collider* _collider)
	{
		AddCollisionCount();

		const GameObject::Scripts& scripts = gameObject()->GetScripts();

		for (size_t i = 0; i < scripts.size(); ++i)
		{
			scripts[i]->OnTriggerEnter(_collider);
		}
	}

	void Collider::OnTriggerStay(Collider* _collider)
	{
		const GameObject::Scripts& scripts = gameObject()->GetScripts();

		for (size_t i = 0; i < scripts.size(); ++i)
		{
			scripts[i]->OnTriggerStay(_collider);
		}
	}

	void Collider::OnTriggerExit(Collider* _collider)
	{
		SubCollisionCount();

		const GameObject::Scripts& scripts = gameObject()->GetScripts();

		for (size_t i = 0; i < scripts.size(); ++i)
		{
			scripts[i]->OnTriggerExit(_collider);
		}
	}

}

