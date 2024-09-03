#include "Engine/Game/Component/Collider/Collider2D.h"

#include "Engine/Game/Component/Com_Transform.h"
#include "Engine/Game/Component/Script/Script.h"

#include "Engine/Game/GameObject.h"
#include "Engine/Game/Scene.h"

#include "Engine/Game/Collision/CollisionSystem.h"
#include "Engine/Game/Collision/Collision2D.h"

#include "Engine/Manager/RenderManager.h"


namespace ehw
{
	Collider2D::Collider2D(const std::string_view key, eCollider2D_Shape _type)
		: Collider(key, eDimensionType::_2D)
		, m_collider2DShape(_type)
		, m_isColliderUpdated(false)
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

	eResult Collider2D::Serialize_Json(JsonSerializer* _ser) const
	{
		return eResult();
	}

	eResult Collider2D::DeSerialize_Json(const JsonSerializer* _ser)
	{
		return eResult();
	}

	void Collider2D::Init()
	{
		Collider::Init();

		if (GetCollisionSystem())
		{
			m_col2dManager = GetCollisionSystem()->GetCollision2D();
		}
		
		if (m_col2dManager)
		{
			m_col2dManager->Enqueue(this);
		}
	}

	void Collider2D::FrameEnd()
	{
		Collider::FrameEnd();
		m_isColliderUpdated = false;
	}
}