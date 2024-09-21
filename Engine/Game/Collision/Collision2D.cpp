#include "Engine/Game/Collision/Collision2D.h"

#include "Engine/Game/Collision/CollisionSystem.h"

#include "Engine/Game/GameObject.h"

#include "Engine/Game/Component/Camera/Com_Camera.h"
#include "Engine/Game/Component/Collider/Collider2D.h"
#include "Engine/Game/Component/Collider/Com_Collider2D_AABB.h"

#include "Engine/Resource/Mesh/Mesh.h"
#include "Engine/Manager/RenderManager.h"
#include "Engine/Manager/ResourceManager.h"



#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

namespace ehw
{
	//AABB,
	//OBB,
	//Circle,
	//END
	std::array<std::array<Collision2D::CheckIntersectFunction,
		(int)eCollider2D_Shape::END>, (int)eCollider2D_Shape::END>
		Collision2D::s_checkIntersectFunctions
	{
		{
			{
				//AABB-AABB(0, 0)
				std::bind(Collision2D::CheckIntersect_AABB_AABB,
					std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),

				//AABB-OBB(0, 1)
				std::bind(Collision2D::CheckIntersect_AABB_OBB,
					std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),

				//AABB-Circle(0, 2)
				std::bind(Collision2D::CheckIntersect_AABB_Circle,
					std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
			}
			,
			{
				//OBB-AABB(1, 0)
				std::bind(Collision2D::CheckIntersect_AABB_OBB,
					std::placeholders::_2, std::placeholders::_1, std::placeholders::_3),

				//OBB-OBB(1, 1)
				std::bind(Collision2D::CheckIntersect_OBB_OBB,
					std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),

				//OBB-Circle(1, 2)
				std::bind(Collision2D::CheckIntersect_OBB_Circle,
					std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
			}
			,
			{
				//Circle-AABB(2, 0)
				std::bind(Collision2D::CheckIntersect_AABB_Circle,
					std::placeholders::_2, std::placeholders::_1, std::placeholders::_3),

				//Circle-OBB(2, 1)
				std::bind(Collision2D::CheckIntersect_OBB_Circle,
					std::placeholders::_2, std::placeholders::_1, std::placeholders::_3),

				//Circle-Circle(2, 2)
				std::bind(Collision2D::CheckIntersect_Circle_Circle,
					std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
			}
		}
	};

	Collision2D::Collision2D(CollisionSystem* const _owner)
		: m_owner(_owner)
		, m_collidersInLayer{}
	{
	}

	Collision2D::~Collision2D()
	{
	}

	void Collision2D::Enqueue(Collider2D* const _obj)
	{
		if (_obj)
		{
			m_collidersInLayer[_obj->gameObject()->GetLayer()].push_back(_obj);
		}
	}

	void Collision2D::FixedUpdate()
	{
		//m_collision 데이터를 임시 컨테이너로 옮겨준다
		std::unordered_map<tColliderID, ContactPair, tColliderID_Hasher> prevCollisions{};
		m_collisions.swap(prevCollisions);

		const auto& colMask = m_owner->GetCollisionMask();
		for (size_t i = 0; i < m_collidersInLayer.size(); ++i)
		{
			for (size_t j = i; j < m_collidersInLayer.size(); ++j)
			{
				//충돌하지 않을 시 break
				if (false == colMask[i][j])
				{
					continue;
				}

				const std::vector<Collider2D*>& lefts = m_collidersInLayer[i];
				const std::vector<Collider2D*>& rights = m_collidersInLayer[j];


				for (size_t l = 0; l < lefts.size(); ++l)
				{
					Collider2D* const left = lefts[l];

					//동일 레이어 간 충돌일 시, r의 시작번호는 i + 1
					size_t r = 0;
					if (i == j)
					{
						r = l +1;
					}

					for (r; r < rights.size(); ++r)
					{
						Collider2D* const right = rights[r];
						
						left->ColliderUpdate();
						right->ColliderUpdate();

						eCollider2D_Shape leftShape = left->GetColliderShape();
						eCollider2D_Shape rightShape = right->GetColliderShape();

						float2 contactPoint{};
						bool isContact = s_checkIntersectFunctions[(int)leftShape][(int)rightShape]
						(left, right, contactPoint);

						tColliderID id{ left->GetID(), right->GetID() };

						//충돌했을 경우: 첫 충돌 함수 호출 또는 계속 충돌 함수 호출
						if (isContact)
						{
							const auto& iter = prevCollisions.find(id);

							//map에 없으면 첫 충돌
							if (prevCollisions.end() == iter)
							{
								ContactPair info{ left, right };
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

	void Collision2D::render()
	{
		////메인 카메라
		//Com_Camera* mainCam = RenderManager::GetInst().sceneRenderAgent().GetMainCamera();
		//if (nullptr == mainCam)
		//{
		//	return;
		//}
		//MATRIX matView = mainCam->GetViewMatrix();
		//MATRIX matProj = mainCam->GetProjectionMatrix();

		////인스턴싱
		//for (size_t i = 0; i < m_debugInstancingData.size(); ++i)
		//{
		//	m_debugInstancingData[i].clear();
		//}

		//for (size_t i = 0; i < m_collidersInLayer.size(); ++i)
		//{
		//	for (size_t j = 0; j < m_collidersInLayer[i].size(); ++j)
		//	{
		//		Collider2D* col = m_collidersInLayer[i][j];

		//		if (false == col->IsEnabled())
		//		{
		//			continue;
		//		}

		//		col->ColliderUpdate();

		//		//TODO: Collider 별로 사이즈 제대로 설정하기
		//		tDebugDrawData debugData{};
		//		debugData.WVP = col->GetColliderMatrix();
		//		debugData.WVP *= matView;
		//		debugData.WVP *= matProj;
		//		debugData.isColliding = (col->IsColliding() ? TRUE : FALSE);

		//		m_debugInstancingData[(int)col->GetColliderShape()].push_back(std::move(debugData));
		//	}
		//}

		//std::shared_ptr<Mesh> mesh = ResourceManager<Mesh>::GetInst().Find(strKey::defaultRes::mesh::DebugRectMesh);


		//m_owner->RenderDebugMesh(mesh, m_debugInstancingData[(int)eCollider2D_Shape::AABB]);

		//m_owner->RenderDebugMesh(mesh, m_debugInstancingData[(int)eCollider2D_Shape::OBB]);

		////mesh = ResourceManager<Mesh>::GetInst().Find(strKey::defaultRes::mesh::CircleMesh);
		////::ehw::RenderDebugMesh(mesh, m_debugInstancingData[(int)eCollider2D_Shape::Circle], m_debug_render_data.get());

		//
	}

	void Collision2D::frame_end()
	{
		for (size_t i = 0; i < m_collidersInLayer.size(); ++i)
		{
			m_collidersInLayer[i].clear();
		}
	}



	bool Collision2D::CheckIntersect_AABB_AABB(Collider2D* const _AABB1, Collider2D* const _AABB2, float2& _contactPoint)
	{
		Com_Collider2D_AABB* colA = static_cast<Com_Collider2D_AABB*>(_AABB1);
		Com_Collider2D_AABB* colB = static_cast<Com_Collider2D_AABB*>(_AABB2);

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


	bool Collision2D::CheckIntersect_AABB_OBB(Collider2D* const _AABB, Collider2D* const _OBB, float2& _contactPoint)
	{
		return false;
	}
	bool Collision2D::CheckIntersect_AABB_Circle(Collider2D* const _AABB, Collider2D* const _circle, float2& _contactPoint)
	{
		return false;
	}
	bool Collision2D::CheckIntersect_OBB_OBB(Collider2D* const _OBB1, Collider2D* const _OBB2, float2& _contactPoint)
	{
		return false;
	}
	bool Collision2D::CheckIntersect_OBB_Circle(Collider2D* const _OBB, Collider2D* const _circle, float2& _contactPoint)
	{
		return false;
	}
	bool Collision2D::CheckIntersect_Circle_Circle(Collider2D* const _circle1, Collider2D* const _circle2, float2& _contactPoint)
	{
		return false;
	}
}
