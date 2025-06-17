#include "Engine/Game/Component/Collider/Collider.h"

#include "Engine/Game/Scene.h"
#include "Engine/Game/GameObject.h"
#include "Engine/Game/Collision/CollisionSystem.h"
#include "Engine/Game/Component/Transform.h"
#include "Engine/Game/Component/Script/Script.h"


namespace core
{
	Collider::Collider(const std::string_view key, eDimensionType _dimension)
		: Super(key, s_component_order)
		, m_dimension(_dimension)
		, m_isTriggerMode(false)
		, m_transform()
		, m_collisionSystem(nullptr)
		, m_collisionCount(0)
	{
	}

	Collider::~Collider()
	{

	}

	void Collider::init()
	{
		m_transform = get_owner()->GetComponent<Transform>();
	}

	void Collider::Awake()
	{
		m_collisionSystem = get_owner()->get_scene()->GetCollisionSystem();
	}

	void Collider::OnCollisionEnter(const s_ptr<Collider>& _collider, const float3& _contactPoint)
	{
		AddCollisionCount();

		const GameObject::Scripts& scripts = get_owner()->GetScripts();

		for (size_t i = 0; i < scripts.size(); ++i)
		{
			scripts[i]->OnCollisionEnter(_collider, _contactPoint);
		}
	}

	void Collider::OnCollisionStay(const s_ptr<Collider>& _collider, const float3& _contactPoint)
	{
		const GameObject::Scripts& scripts = get_owner()->GetScripts();

		for (size_t i = 0; i < scripts.size(); ++i)
		{
			scripts[i]->OnCollisionStay(_collider, _contactPoint);
		}
	}

	void Collider::OnCollisionExit(const s_ptr<Collider>& _collider)
	{
		SubCollisionCount();

		const GameObject::Scripts& scripts = get_owner()->GetScripts();

		for (size_t i = 0; i < scripts.size(); ++i)
		{
			scripts[i]->OnCollisionExit(_collider);
		}
	}

	void Collider::OnTriggerEnter(const s_ptr<Collider>& _collider)
	{
		AddCollisionCount();

		const GameObject::Scripts& scripts = get_owner()->GetScripts();

		for (size_t i = 0; i < scripts.size(); ++i)
		{
			scripts[i]->OnTriggerEnter(_collider);
		}
	}

	void Collider::OnTriggerStay(const s_ptr<Collider>& _collider)
	{
		const GameObject::Scripts& scripts = get_owner()->GetScripts();

		for (size_t i = 0; i < scripts.size(); ++i)
		{
			scripts[i]->OnTriggerStay(_collider);
		}
	}

	void Collider::OnTriggerExit(const s_ptr<Collider>& _collider)
	{
		SubCollisionCount();

		const GameObject::Scripts& scripts = get_owner()->GetScripts();

		for (size_t i = 0; i < scripts.size(); ++i)
		{
			scripts[i]->OnTriggerExit(_collider);
		}
	}

}

