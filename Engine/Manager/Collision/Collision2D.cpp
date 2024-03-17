#include "Engine/Manager/Collision/Collision2D.h"

#include "Engine/Manager/CollisionManager.h"

#include "Engine/Game/GameObject.h"
#include "Engine/Game/Component/Collider/iCollider2D.h"

#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

namespace ehw
{
	void Collision2D::Enqueue(const std::shared_ptr<iCollider2D>& _obj)
	{
		if (nullptr == _obj)
		{
			return;
		}
		m_objectsInLayer[_obj->GetOwner()->GetLayer()].push_back(_obj);
	}

	Collision2D::Collision2D()
		: m_objectsInLayer{}
	{
	}

	Collision2D::~Collision2D()
	{
	}



	void Collision2D::Init()
	{
		//AABB-AABB
		m_collision2DFunctions[(int)eCollider2D_Shape::AABB][(int)eCollider2D_Shape::AABB]
			= std::bind(&Collision2D::CheckIntersect_AABB_AABB, this, 
				std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		//AABB-Circle
		m_collision2DFunctions[(int)eCollider2D_Shape::AABB][(int)eCollider2D_Shape::Circle]
			= std::bind(&Collision2D::CheckIntersect_AABB_Circle, this, 
				std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		m_collision2DFunctions[(int)eCollider2D_Shape::Circle][(int)eCollider2D_Shape::AABB]
			= std::bind(&Collision2D::CheckIntersect_AABB_Circle, this, 
				std::placeholders::_2, std::placeholders::_1, std::placeholders::_3);

		//AABB-OBB
		m_collision2DFunctions[(int)eCollider2D_Shape::AABB][(int)eCollider2D_Shape::OBB]
			= std::bind(&Collision2D::CheckIntersect_AABB_OBB, this, 
				std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		m_collision2DFunctions[(int)eCollider2D_Shape::OBB][(int)eCollider2D_Shape::AABB]
			= std::bind(&Collision2D::CheckIntersect_AABB_OBB, this, 
				std::placeholders::_2, std::placeholders::_1, std::placeholders::_3);

		//Circle-Circle
		m_collision2DFunctions[(int)eCollider2D_Shape::Circle][(int)eCollider2D_Shape::Circle]
			= std::bind(&Collision2D::CheckIntersect_Circle_Circle, this,
				std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		//Circle-OBB
		m_collision2DFunctions[(int)eCollider2D_Shape::OBB][(int)eCollider2D_Shape::Circle]
			= std::bind(&Collision2D::CheckIntersect_OBB_Circle, this,
				std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		m_collision2DFunctions[(int)eCollider2D_Shape::Circle][(int)eCollider2D_Shape::OBB]
			= std::bind(&Collision2D::CheckIntersect_OBB_Circle, this,
				std::placeholders::_2, std::placeholders::_1, std::placeholders::_3);

		//OBB-OBB
		m_collision2DFunctions[(int)eCollider2D_Shape::OBB][(int)eCollider2D_Shape::OBB]
			= std::bind(&Collision2D::CheckIntersect_OBB_OBB, this,
				std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	}


	void Collision2D::FinalUpdate()
	{
		const auto& colMask = CollisionManager::GetCollisionMasks();
		for (size_t i = 0; i < m_objectsInLayer.size(); ++i)
		{
			for (size_t j = i + 1; j < m_objectsInLayer.size(); ++j)
			{
				//충돌하지 않을 시 break
				if (false == colMask[i][j])
				{
					continue;
				}

				const std::vector<std::shared_ptr<iCollider2D>>& lefts = m_objectsInLayer[i];
				const std::vector<std::shared_ptr<iCollider2D>>& rights = m_objectsInLayer[j];

				for (size_t l = 0; l < lefts.size(); ++l)
				{
					for (size_t r = 0; r < rights.size(); ++r)
					{
						const std::shared_ptr<iCollider2D>& left = lefts[l];
						const std::shared_ptr<iCollider2D>& right = rights[r];

						int leftShape = (int)left->GetColliderShape();
						int rightShape = (int)right->GetColliderShape();
						Vector2 hitPoint{};

						bool isIntercept = m_collision2DFunctions[leftShape][rightShape]
						(left, right, hitPoint);

						//충돌체 ID 생성 및 충돌체 정보 찾아오기
						union_ColliderID colID{ left->GetID(), right->GetID() };
						//const auto& colInfo = m_collisionMap.find(colID);

						//충돌했을 경우: 첫 충돌 함수 호출 또는 계속 충돌 함수 호출
						if (isIntercept)
						{
							ASSERT(false, "미구현");
						}

					}
				}
			}
		}
	}

	void Collision2D::FrameEnd()
	{
		Reset();
	}
	void Collision2D::Reset()
	{
		for (size_t i = 0; i < m_objectsInLayer.size(); ++i)
		{
			m_objectsInLayer[i].clear();
		}
	}

	bool Collision2D::CheckIntersect_AABB_AABB(const std::shared_ptr<iCollider2D>& _AABB1, const std::shared_ptr<iCollider2D>& _AABB2, Vector2& _hitPoint)
	{
		return false;
	}
	bool Collision2D::CheckIntersect_AABB_OBB(const std::shared_ptr<iCollider2D>& _AABB, const std::shared_ptr<iCollider2D>& _OBB, Vector2& _hitPoint)
	{
		return false;
	}
	bool Collision2D::CheckIntersect_AABB_Circle(const std::shared_ptr<iCollider2D>& _AABB, const std::shared_ptr<iCollider2D>& _circle, Vector2& _hitPoint)
	{
		return false;
	}
	bool Collision2D::CheckIntersect_OBB_OBB(const std::shared_ptr<iCollider2D>& _OBB1, const std::shared_ptr<iCollider2D>& _OBB2, Vector2& _hitPoint)
	{
		return false;
	}
	bool Collision2D::CheckIntersect_OBB_Circle(const std::shared_ptr<iCollider2D>& _OBB, const std::shared_ptr<iCollider2D>& _circle, Vector2& _hitPoint)
	{
		return false;
	}
	bool Collision2D::CheckIntersect_OBB_AABB(const std::shared_ptr<iCollider2D>& _OBB, const std::shared_ptr<iCollider2D>& _AABB, Vector2& _hitPoint)
	{
		return false;
	}
	bool Collision2D::CheckIntersect_Circle_Circle(const std::shared_ptr<iCollider2D>& _circle1, const std::shared_ptr<iCollider2D>& _circle2, Vector2& _hitPoint)
	{
		return false;
	}
}
