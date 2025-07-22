#include "Collider.h"

#include <Engine/Game/Scene.h>
#include <Engine/Game/GameObject.h>
#include <Engine/Game/Collision/CollisionSystem.h>
#include <Engine/Game/Component/Transform.h>
#include <Engine/Game/Component/Script/Script.h>


namespace core
{
	Collider::Collider(eDimensionType _dimension)
		: Super(s_component_order)
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
		Super::init();
		m_transform = get_owner()->get_component<Transform>();
	}

	void Collider::awake()
	{
		m_collisionSystem = get_owner()->get_scene()->get_collision_system();
	}

	void Collider::on_collision_enter(const s_ptr<Collider>& _collider, const float3& _contactPoint)
	{
		add_collision_count();

		const GameObject::Scripts& scripts = get_owner()->get_scripts();

		for (size_t i = 0; i < scripts.size(); ++i)
		{
			scripts[i]->on_collision_enter(_collider, _contactPoint);
		}
	}

	void Collider::on_collision_stay(const s_ptr<Collider>& _collider, const float3& _contactPoint)
	{
		const GameObject::Scripts& scripts = get_owner()->get_scripts();

		for (size_t i = 0; i < scripts.size(); ++i)
		{
			scripts[i]->on_collision_stay(_collider, _contactPoint);
		}
	}

	void Collider::on_collision_exit(const s_ptr<Collider>& _collider)
	{
		sub_collision_count();

		const GameObject::Scripts& scripts = get_owner()->get_scripts();

		for (size_t i = 0; i < scripts.size(); ++i)
		{
			scripts[i]->on_collision_exit(_collider);
		}
	}

	void Collider::on_trigger_enter(const s_ptr<Collider>& _collider)
	{
		add_collision_count();

		const GameObject::Scripts& scripts = get_owner()->get_scripts();

		for (size_t i = 0; i < scripts.size(); ++i)
		{
			scripts[i]->on_trigger_enter(_collider);
		}
	}

	void Collider::on_trigger_stay(const s_ptr<Collider>& _collider)
	{
		const GameObject::Scripts& scripts = get_owner()->get_scripts();

		for (size_t i = 0; i < scripts.size(); ++i)
		{
			scripts[i]->on_trigger_stay(_collider);
		}
	}

	void Collider::on_trigger_exit(const s_ptr<Collider>& _collider)
	{
		sub_collision_count();

		const GameObject::Scripts& scripts = get_owner()->get_scripts();

		for (size_t i = 0; i < scripts.size(); ++i)
		{
			scripts[i]->on_trigger_exit(_collider);
		}
	}

}

