#include "Engine/Game/Collision/Collision2D.h"

#include "Engine/Game/Collision/CollisionSystem.h"
#include "Engine/Manager/RenderManager.h"
#include "Engine/Manager/ResourceManager.h"

#include "Engine/Resource/Mesh.h"
#include "Engine/Resource/Material.h"

#include "Engine/GPU/StructBuffer.h"

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
					std::placeholders::_1, std::placeholders::_2),

				//AABB-OBB(0, 1)
				std::bind(Collision2D::CheckIntersect_AABB_OBB,
					std::placeholders::_1, std::placeholders::_2),

				//AABB-Circle(0, 2)
				std::bind(Collision2D::CheckIntersect_AABB_Circle,
					std::placeholders::_1, std::placeholders::_2)
			}
			,
			{
				//OBB-AABB(1, 0)
				std::bind(Collision2D::CheckIntersect_AABB_OBB,
					std::placeholders::_2, std::placeholders::_1),

				//OBB-OBB(1, 1)
				std::bind(Collision2D::CheckIntersect_OBB_OBB,
					std::placeholders::_1, std::placeholders::_2),

				//OBB-Circle(1, 2)
				std::bind(Collision2D::CheckIntersect_OBB_Circle,
					std::placeholders::_1, std::placeholders::_2)
			}
			,
			{
				//Circle-AABB(2, 0)
				std::bind(Collision2D::CheckIntersect_AABB_Circle,
					std::placeholders::_2, std::placeholders::_1),

				//Circle-OBB(2, 1)
				std::bind(Collision2D::CheckIntersect_OBB_Circle,
					std::placeholders::_2, std::placeholders::_1),

				//Circle-Circle(2, 2)
				std::bind(Collision2D::CheckIntersect_Circle_Circle,
					std::placeholders::_1, std::placeholders::_2),
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
	void Collision2D::Reset()
	{
		for (size_t i = 0; i < m_collidersInLayer.size(); ++i)
		{
			m_collidersInLayer[i].clear();
		}

		m_collisions.clear();

		m_debugInfoSBuffer = nullptr;

		for (size_t i = 0; i < m_debugInstancingData.size(); ++i)
		{
			m_debugInstancingData[i].clear();
		}

		m_debugInfoSBuffer = nullptr;
	}

	void Collision2D::Enqueue(iCollider2D* const _obj)
	{
		if (_obj)
		{
			m_collidersInLayer[_obj->GetOwner()->GetLayer()].push_back(_obj);
		}
	}



	void Collision2D::Update()
	{
		//m_collision 데이터를 임시 컨테이너로 옮겨준다
		std::unordered_map<tColliderID, CollisionInfo, tColliderID_Hasher> prevCollisions{};
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

				const std::vector<iCollider2D*>& lefts = m_collidersInLayer[i];
				const std::vector<iCollider2D*>& rights = m_collidersInLayer[j];


				for (size_t l = 0; l < lefts.size(); ++l)
				{
					iCollider2D* const left = lefts[l];

					//동일 레이어 간 충돌일 시, r의 시작번호는 i + 1
					size_t r = 0;
					if (i == j)
					{
						r = l +1;
					}

					for (r; r < rights.size(); ++r)
					{
						iCollider2D* const right = rights[r];
						
						left->ColliderUpdate();
						right->ColliderUpdate();

						eCollider2D_Shape leftShape = left->GetColliderShape();
						eCollider2D_Shape rightShape = right->GetColliderShape();

						bool isContact = s_checkIntersectFunctions[(int)leftShape][(int)rightShape]
						(left, right);

						tColliderID id{ left->GetID(), right->GetID() };

						//충돌했을 경우: 첫 충돌 함수 호출 또는 계속 충돌 함수 호출
						if (isContact)
						{
							left->SetColliding(true);
							right->SetColliding(true);

							const auto& iter = prevCollisions.find(id);

							//map에 없으면 첫 충돌
							if (prevCollisions.end() == iter)
							{
								CollisionInfo info{ left, right };
								info.Enter();

								m_collisions.insert(
									std::make_pair(std::move(id), std::move(info)));
							}
							else
							{
								//Stay 호출
								iter->second.Stay();

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

	void Collision2D::Render()
	{
		//메인 카메라
		Com_Camera* mainCam = RenderManager::GetMainCamera();
		if (nullptr == mainCam)
		{
			return;
		}
		MATRIX matView = mainCam->GetGpuViewMatrix();
		MATRIX matProj = mainCam->GetGpuProjectionMatrix();

		//인스턴싱
		for (size_t i = 0; i < m_debugInstancingData.size(); ++i)
		{
			m_debugInstancingData[i].clear();
		}

		for (size_t i = 0; i < m_collidersInLayer.size(); ++i)
		{
			for (size_t j = 0; j < m_collidersInLayer[i].size(); ++j)
			{
				iCollider2D* col = m_collidersInLayer[i][j];

				if (false == col->IsEnabled())
				{
					continue;
				}

				col->ColliderUpdate();

				//TODO: Collider 별로 사이즈 제대로 설정하기
				tDebugDrawData debugData{};
				debugData.WVP = col->GetColliderMatrix();
				debugData.WVP *= matView;
				debugData.WVP *= matProj;
				debugData.isColliding = (col->IsColliding() ? TRUE : FALSE);

				m_debugInstancingData[(int)col->GetColliderShape()].push_back(std::move(debugData));
			}
		}


		//구조화 버퍼 없을 시 생성
		if (nullptr == m_debugInfoSBuffer)
		{
			size_t maxCount = 0;
			for (size_t i = 0; i < m_debugInstancingData.size(); ++i)
			{
				maxCount += m_debugInstancingData[i].size();
			}
			if ((size_t)0 == maxCount)
			{
				maxCount = 1;
			}

			m_debugInfoSBuffer = std::make_unique<StructBuffer>();
			StructBuffer::Desc desc{};
			desc.eSBufferType = eStructBufferType::READ_ONLY;
			desc.REGISLOT_t_SRV = Register_t_g_debugDrawData;
			desc.TargetStageSRV = eShaderStageFlag::Vertex | eShaderStageFlag::Pixel;
			m_debugInfoSBuffer->SetDesc(desc);
			m_debugInfoSBuffer->Create<tDebugDrawData>(maxCount, nullptr, 0);
		}

		std::shared_ptr<Mesh> mesh = ResourceManager<Mesh>::Find(strKey::defaultRes::mesh::DebugRectMesh);
		std::shared_ptr<Material> mtrl = ResourceManager<Material>::Find(strKey::defaultRes::material::DebugMaterial);
		mtrl->BindData();

		RenderDebugMesh(mesh, m_debugInstancingData[(int)eCollider2D_Shape::AABB], m_debugInfoSBuffer.get());

		RenderDebugMesh(mesh, m_debugInstancingData[(int)eCollider2D_Shape::OBB], m_debugInfoSBuffer.get());

		//mesh = ResourceManager<Mesh>::Find(strKey::defaultRes::mesh::CircleMesh);
		//::ehw::RenderDebugMesh(mesh, m_debugInstancingData[(int)eCollider2D_Shape::Circle], m_debugInfoSBuffer.get());

		mtrl->UnBindData();
	}

	void Collision2D::FrameEnd()
	{
		for (size_t i = 0; i < m_collidersInLayer.size(); ++i)
		{
			m_collidersInLayer[i].clear();
		}
	}



	bool Collision2D::CheckIntersect_AABB_AABB(iCollider2D* const _AABB1, iCollider2D* const _AABB2)
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

	void Collision2D::RenderDebugMesh(const std::shared_ptr<Mesh>& _mesh, const std::vector<tDebugDrawData>& _debugData, StructBuffer* const _sbuffer)
	{
		_sbuffer->SetData(static_cast<const void*>(_debugData.data()), _debugData.size());
		_sbuffer->BindDataSRV();
		_mesh->RenderInstanced(0, (UINT)_debugData.size());
	}

	bool Collision2D::CheckIntersect_AABB_OBB(iCollider2D* const _AABB, iCollider2D* const _OBB)
	{
		return false;
	}
	bool Collision2D::CheckIntersect_AABB_Circle(iCollider2D* const _AABB, iCollider2D* const _circle)
	{
		return false;
	}
	bool Collision2D::CheckIntersect_OBB_OBB(iCollider2D* const _OBB1, iCollider2D* const _OBB2)
	{
		return false;
	}
	bool Collision2D::CheckIntersect_OBB_Circle(iCollider2D* const _OBB, iCollider2D* const _circle)
	{
		return false;
	}
	bool Collision2D::CheckIntersect_Circle_Circle(iCollider2D* const _circle1, iCollider2D* const _circle2)
	{
		return false;
	}
}
