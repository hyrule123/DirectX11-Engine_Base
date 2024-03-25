#include "Engine/Manager/Collision/Collision2D.h"

#include "Engine/Manager/CollisionManager.h"
#include "Engine/Manager/RenderManager.h"
#include "Engine/Manager/ResourceManager.h"

#include "Engine/Resource/Mesh.h"
#include "Engine/Resource/Material.h"

#include "Engine/GPU/StructBuffer.h"

#include "Engine/Game/GameObject.h"
#include "Engine/Game/Component/Collider/iCollider2D.h"
#include "Engine/Game/Component/Collider/Com_Collider2D_AABB.h"

#include "Engine/DefaultShader/Debug/Debug.hlsli"


#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

namespace ehw
{
	//CPP 내부용 전역변수
	void RenderDebugMesh(const std::shared_ptr<Mesh>& _mesh, const std::vector<tDebug>& _debugData, StructBuffer* const _sbuffer);

	Collision2D::Collision2D()
		: m_objectsInLayer{}
		, m_colliders{}
	{
	}

	Collision2D::~Collision2D()
	{
	}
	void Collision2D::Reset()
	{
		for (size_t i = 0; i < m_objectsInLayer.size(); ++i)
		{
			m_objectsInLayer[i].clear();
		}
		m_colliders.clear();

		m_collisions.clear();

		m_debugInfoSBuffer = nullptr;
	}

	void Collision2D::Register(iCollider2D* const _obj)
	{
		if (nullptr == _obj)
		{
			return;
		}
		m_colliders.push_back(_obj);
	}
	void Collision2D::Enqueue(iCollider2D* const _obj)
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
			[this](iCollider2D* const _col)->bool
			{
				if (_col->IsDestroyed())
				{
					return true;
				}
				else if (false == _col->IsEnabled())
				{
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

				const std::vector<iCollider2D*>& lefts = m_objectsInLayer[i];
				const std::vector<iCollider2D*>& rights = m_objectsInLayer[j];


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

						eCollider2D_Shape leftShape = left->GetColliderShape();
						eCollider2D_Shape rightShape = right->GetColliderShape();
						Vector2 contactPoint{};

						bool isContact = m_collision2DFunctions[(int)leftShape][(int)rightShape]
						(left, right, contactPoint);

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

	void Collision2D::Render()
	{
		//미리 collider 최대 갯수만큼 사이즈를 확보
		std::array<std::vector<tDebug>, (int)eCollider2D_Shape::END> debugInstancingData{};
		for (size_t i = 0; i < debugInstancingData.size(); ++i)
		{
			debugInstancingData[i].reserve(m_colliders.size());
		}

		MATRIX matView = RenderManager::GetMainCamera()->GetGpuViewMatrix();
		MATRIX matProj = RenderManager::GetMainCamera()->GetGpuProjectionMatrix();

		for (size_t i = 0; i < m_colliders.size(); ++i)
		{
			tDebug debugData{};
			debugData.WVP = MATRIX::CreateScale(100.f) * m_colliders[i]->GetWorldMatrix();
			debugData.WVP *= matView;
			debugData.WVP *= matProj;
			debugData.isColliding = (m_colliders[i]->IsColliding() ? TRUE : FALSE);

			debugInstancingData[(int)m_colliders[i]->GetColliderShape()].push_back(std::move(debugData));
		}

		if (nullptr == m_debugInfoSBuffer)
		{
			m_debugInfoSBuffer = std::make_unique<StructBuffer>();
			StructBuffer::Desc desc{};
			desc.eSBufferType = eStructBufferType::READ_ONLY;
			desc.REGISLOT_t_SRV = Register_t_g_debug;
			desc.TargetStageSRV = eShaderStageFlag::Vertex | eShaderStageFlag::Pixel;
			m_debugInfoSBuffer->SetDesc(desc);
			m_debugInfoSBuffer->Create<tDebug>(m_colliders.size(), nullptr, 0);
		}

		std::shared_ptr<Mesh> mesh = ResourceManager<Mesh>::Find(strKey::defaultRes::mesh::DebugRectMesh);
		std::shared_ptr<Material> mtrl = ResourceManager<Material>::Find(strKey::defaultRes::material::DebugMaterial);
		mtrl->BindData();

		::ehw::RenderDebugMesh(mesh, debugInstancingData[(int)eCollider2D_Shape::AABB], m_debugInfoSBuffer.get());

		::ehw::RenderDebugMesh(mesh, debugInstancingData[(int)eCollider2D_Shape::OBB], m_debugInfoSBuffer.get());

		//mesh = ResourceManager<Mesh>::Find(strKey::defaultRes::mesh::CircleMesh);
		//::ehw::RenderDebugMesh(mesh, debugInstancingData[(int)eCollider2D_Shape::Circle], m_debugInfoSBuffer.get());

		mtrl->UnBindData();
	}



	bool Collision2D::CheckIntersect_AABB_AABB(iCollider2D* const _AABB1, iCollider2D* const _AABB2, Vector2& _hitPoint)
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

	//전역 함수
	void ::ehw::RenderDebugMesh(const std::shared_ptr<Mesh>& _mesh, const std::vector<tDebug>& _debugData, StructBuffer* const _sbuffer)
	{
		_sbuffer->SetData(static_cast<const void*>(_debugData.data()), _debugData.size());
		_sbuffer->BindDataSRV();
		_mesh->RenderInstanced(0, (UINT)_debugData.size());
	}

	bool Collision2D::CheckIntersect_AABB_OBB(iCollider2D* const _AABB, iCollider2D* const _OBB, Vector2& _hitPoint)
	{
		return false;
	}
	bool Collision2D::CheckIntersect_AABB_Circle(iCollider2D* const _AABB, iCollider2D* const _circle, Vector2& _hitPoint)
	{
		return false;
	}
	bool Collision2D::CheckIntersect_OBB_OBB(iCollider2D* const _OBB1, iCollider2D* const _OBB2, Vector2& _hitPoint)
	{
		return false;
	}
	bool Collision2D::CheckIntersect_OBB_Circle(iCollider2D* const _OBB, iCollider2D* const _circle, Vector2& _hitPoint)
	{
		return false;
	}
	bool Collision2D::CheckIntersect_OBB_AABB(iCollider2D* const _OBB, iCollider2D* const _AABB, Vector2& _hitPoint)
	{
		return false;
	}
	bool Collision2D::CheckIntersect_Circle_Circle(iCollider2D* const _circle1, iCollider2D* const _circle2, Vector2& _hitPoint)
	{
		return false;
	}
}
