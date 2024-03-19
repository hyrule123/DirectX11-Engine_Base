#include "Engine/Manager/Collision/Collision2D.h"

#include "Engine/Manager/CollisionManager.h"

#include "Engine/Game/GameObject.h"
#include "Engine/Game/Component/Collider/iCollider2D.h"
#include "Engine/Game/Component/Collider/Com_Collider2D_AABB.h"

#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

namespace ehw
{
	Collision2D::Collision2D()
		: m_objectsInLayer{}
		, m_colliders{}
	{
	}

	Collision2D::~Collision2D()
	{
	}

	void Collision2D::Register(const std::shared_ptr<iCollider2D>& _obj)
	{
		if (nullptr == _obj)
		{
			return;
		}
		m_colliders.push_back(_obj);
	}
	void Collision2D::Enqueue(const std::shared_ptr<iCollider2D>& _obj)
	{
		m_objectsInLayer[_obj->GetOwner()->GetLayer()].push_back(_obj);
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


	void Collision2D::Update()
	{
		for (size_t i = 0; i < m_objectsInLayer.size(); ++i)
		{
			m_objectsInLayer[i].clear();
		}

		//Destroy된 component를 제거해주면서 Enqueue 작업까지 진행한다.
		std::erase_if(m_colliders,
			[this](const std::shared_ptr<iCollider2D>& _col)->bool
			{
				if (_col->IsDestroyed())
				{
					_col->AllContactsExit();
					return true;
				}
				else if (false == _col->IsEnabled())
				{
					_col->AllContactsExit();
					return false;
				}
				
				Enqueue(_col);
				return false;
			}
			);

		//m_collision 데이터를 임시 컨테이너로 옮겨준다
		std::unordered_map<tColliderID, CollisionInfo, tColliderID_Hasher> prevCollisions{};
		m_collisions.swap(prevCollisions);

		const auto& colMask = CollisionManager::GetCollisionMasks();
		for (size_t i = 0; i < m_objectsInLayer.size(); ++i)
		{
			for (size_t j = i; j < m_objectsInLayer.size(); ++j)
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
					const std::shared_ptr<iCollider2D>& left = lefts[l];

					//동일 레이어 간 충돌일 시, r의 시작번호는 i + 1
					size_t r = 0;
					if (i == j)
					{
						r = l +1;
					}

					for (r; r < rights.size(); ++r)
					{
						const std::shared_ptr<iCollider2D>& right = rights[r];

						eCollider2D_Shape leftShape = left->GetColliderShape();
						eCollider2D_Shape rightShape = right->GetColliderShape();
						Vector2 contactPoint{};

						bool isContact = m_collision2DFunctions[(int)leftShape][(int)rightShape]
						(left, right, contactPoint);

						tColliderID id{ left->GetID(), right->GetID() };

						//충돌했을 경우: 첫 충돌 함수 호출 또는 계속 충돌 함수 호출
						if (isContact)
						{
							const auto& iter = prevCollisions.find(id);

							//map에 없으면 첫 충돌
							if (prevCollisions.end() == iter)
							{
								CollisionInfo info{ left, right };
								info.Enter(contactPoint);

								m_collisions.insert(
									std::make_pair(std::move(id), std::move(info)));
							}
							else
							{
								//Stay 호출
								iter->second.Stay(contactPoint);

								//prevCollisions에서 데이터 추출해서 m_collisions로 넣어준다.
								m_collisions.insert(prevCollisions.extract(iter));
							}
						}

						//이러면 prevCollisions 컨테이너에는 이번 충돌에 충돌하지 않은 충돌체들만 남게된다.
						//소멸자에서 Exit() 함수가 호출될것임.
					}
				}
			}
		}


		//이번 충돌검사에서 충돌하지 않았던 충돌들은 충돌 해제 처리를 해준다.
		for (auto& iter : prevCollisions)
		{
			iter.second.Exit();
		}
	}

	void Collision2D::Reset()
	{
		for (size_t i = 0; i < m_objectsInLayer.size(); ++i)
		{
			m_objectsInLayer[i].clear();
		}
		m_colliders.clear();
	}

	bool Collision2D::CheckIntersect_AABB_AABB(const std::shared_ptr<iCollider2D>& _AABB1, const std::shared_ptr<iCollider2D>& _AABB2, Vector2& _hitPoint)
	{
		Com_Collider2D_AABB* colA = static_cast<Com_Collider2D_AABB*>(_AABB1.get());
		Com_Collider2D_AABB* colB = static_cast<Com_Collider2D_AABB*>(_AABB2.get());

		if (colA->Left() > colB->Right())
		{
			return false;
		}
		else if (colA->Bottom() > colB->Top())
		{
			return false;
		}
		else if (colA->Right() < colB->Left())
		{
			return false;
		}
		else if (colA->Top() < colB->Bottom())
		{
			return false;
		}


		return true;
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
