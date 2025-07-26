#include "Collider2D.h"

#include <Engine/Game/Component/Transform.h>
#include <Engine/Game/Component/Script/Script.h>

#include <Engine/Game/GameObject.h>
#include <Engine/Game/Scene.h>

#include <Engine/Game/Collision/CollisionSystem.h>
#include <Engine/Game/Collision/Collision2D.h>

#include <Engine/Manager/RenderManager.h>


namespace core
{
	Collider2D::Collider2D(const std::string_view key, eCollider2D_Shape _type)
		: Collider(eDimensionType::_2D)
		, m_collider_shape_2D(_type)
		, m_b_collider_updated(false)
	{
	}

	Collider2D::Collider2D(const Collider2D& _collider)
		: Collider(_collider)
	{
	}

	Collider2D::~Collider2D()
	{
		//제거는 CollisionSystem::Update에서 일괄적으로 진행됨
		//각자 소멸자에서 제거요청을 보낼경우 매번 vector를 순회돌면서 지워야해서 속도가 많이 느림
	}

	void Collider2D::init()
	{
		Collider::init();

		if (get_collision_system())
		{
			m_collision_manager_2D = get_collision_system()->GetCollision2D();
		}
		
		if (m_collision_manager_2D)
		{
			m_collision_manager_2D->Enqueue(std::static_pointer_cast<Collider2D>(this->shared_from_this()));
		}
	}

	void Collider2D::frame_end()
	{
		Collider::frame_end();
		m_b_collider_updated = false;
	}
}
