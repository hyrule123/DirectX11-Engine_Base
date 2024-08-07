#include "Engine/Scene/Collision/CollisionSystem.h"

#include "Engine/Scene/Scene.h"
#include "Engine/Scene/Component/Transform/Com_Transform.h"
#include "Engine/Scene/Component/Collider/Collider2D.h"
#include "Engine/Scene/Collision/Collision2D.h"
#include "Engine/Scene/Collision/Collision3D.h"

#include "Engine/Manager/ResourceManager.h"

#include "Engine/Resource/Mesh.h"
#include "Engine/Resource/Material.h"

#include "Engine/GPU/StructBuffer.h"

#include "Engine/Util/AtExit.h"

#include "Engine/Manager/SceneManager.h"

namespace ehw
{
	CollisionSystem::CollisionSystem(Scene* const _ownerScene)
		: m_owner{ _ownerScene }
		, m_collisionMask{}
		, m_raycastMask{}
		, m_col2DManager{ nullptr }
		, m_col3DManager{ nullptr }
	{
	}

	CollisionSystem::~CollisionSystem()
	{
	}

	void CollisionSystem::CreateCollision2D()
	{
		m_col2DManager = std::make_unique<Collision2D>(this);
	}

	void CollisionSystem::CreateCollision3D()
	{
		m_col3DManager = std::make_unique<Collision3D>(this);
	}

	void CollisionSystem::FixedUpdate()
	{
		if (m_col2DManager)
		{
			m_col2DManager->FixedUpdate();
		}
		
		if (m_col3DManager)
		{
			m_col3DManager->FixedUpdate();
		}

		
		//Scene* scene = SceneManager::GetActiveScene();
		//if (nullptr == scene)
		//{
		//	return;
		//}

		//for (uint row = 0; row < g_maxLayer; row++)
		//{
		//	for (uint column = 0; column < g_maxLayer; column++)
		//	{
		//		if (m_collisionMask[row][column])
		//		{
		//			LayerCollision(scene, row, column);
		//		}
		//	}
		//}
	}

	void CollisionSystem::FrameEnd()
	{
		if (m_col2DManager) {
			m_col2DManager->FrameEnd();
		}
		
		if (m_col3DManager) {
			m_col3DManager->FrameEnd();
		}
	}

	void CollisionSystem::PrepareDebugRender()
	{
		m_debugInfoSBuffer = std::make_unique<StructBuffer>();
		StructBuffer::Desc desc{};
		desc.eSBufferType = eStructBufferType::READ_ONLY;
		desc.REGISLOT_t_SRV = GPU::Register::t::g_debugDrawData;
		desc.TargetStageSRV = eShaderStageFlag::Vertex | eShaderStageFlag::Pixel;

		m_debugInfoSBuffer->Init<tDebugDrawData>(desc, 100, nullptr, 0);
		ASSERT(m_debugInfoSBuffer.get(), "Debug용 Struct Buffer 생성 실패.");

		m_debugMaterial = ResourceManager<Material>::GetInst().Find(strKey::defaultRes::material::DebugMaterial);
		ASSERT(m_debugMaterial.get(), "DebugMaterial 준비 실패");
	}

	void CollisionSystem::Render() {
		if (m_col2DManager) {
			m_col2DManager->Render();
		}
		if (m_col3DManager) {
			m_col3DManager->Render();
		}
	}

	void CollisionSystem::RenderDebugMesh(const std::shared_ptr<Mesh>& _mesh, const std::vector<tDebugDrawData>& _debugData)
	{
		if (nullptr == m_debugInfoSBuffer)
		{
			PrepareDebugRender();
		}
		else if (nullptr == _mesh || nullptr == m_debugMaterial || nullptr == m_debugInfoSBuffer)
		{
			return;
		}
		
		
		m_debugMaterial->BindData();

		m_debugInfoSBuffer->SetData(static_cast<const void*>(_debugData.data()), _debugData.size());
		m_debugInfoSBuffer->BindDataSRV();

		_mesh->RenderInstanced(0, (UINT)_debugData.size());

		//m_debugMaterial->UnbindData();
	}



	//void CollisionSystem::LayerCollision(Scene* _scene, uint32 _left, uint32 _right)
	//{
		//const std::vector<std::shared_ptr<GameObject>>& lefts = _scene->GetGameObjects(_left);
		//const std::vector<std::shared_ptr<GameObject>>& rights = _scene->GetGameObjects(_right);

		//for (const std::shared_ptr<GameObject>& left : lefts)
		//{
		//	if (false == left->IsActive())
		//		continue;
		//	if (left->GetComponent<Collider2D>() == nullptr)
		//		continue;

		//	for (const std::shared_ptr<GameObject>& right : rights)
		//	{
		//		if (false == left->IsActive())
		//			continue;
		//		if (right->GetComponent<Collider2D>() == nullptr)
		//			continue;
		//		/*if (left == right) //지워도 상관없어서 주석처리 (오류나면 수정)
		//			continue;*/

		//		ColliderCollision(left->GetComponent<Collider2D>(), right->GetComponent<Collider2D>());
		//	}

		//	/*if ((uint)left == (uint)right)  
		//		break;*/
		//}

	//}

	//void CollisionSystem::ColliderCollision(const std::shared_ptr<Collider2D>& _left, const std::shared_ptr<Collider2D>& _right)
	//{
		//// 두 충돌체 레이어로 구성된 ID 확인
		//union_ColliderID colliderID;
		//colliderID.Left = (uint)_left->GetID();
		//colliderID.Right = (uint)_right->GetID();

		//// 이전 충돌 정보를 검색한다.
		//// 만약에 충돌정보가 없는 상태라면
		//// 충돌정보를 생성해준다.
		//std::map<UINT64, bool>::iterator iter = mCollisionMap.find(colliderID.Id);
		//if (iter == mCollisionMap.end())
		//{
		//	mCollisionMap.insert(std::make_pair(colliderID.Id, false));
		//	iter = mCollisionMap.find(colliderID.Id);
		//}

		//// 충돌체크를 해준다.
		//if (Intersect(_left, _right)) // 충돌을 한 상태
		//{
		//	// 최초 충돌중 Enter
		//	if (iter->second == false)
		//	{
		//		if (_left->IsTriiger())
		//		{
		//			_left->OnTriggerEnter(_right);
		//		}
		//		else
		//		{
		//			_left->OnCollisionEnter(_right);
		//		}
		//			

		//		if (_right->IsTriiger())
		//		{
		//			_right->OnTriggerEnter(_left);
		//		}
		//		else
		//		{
		//			_right->OnCollisionEnter(_left);
		//		}

		//		iter->second = true;
		//	}
		//	else // 충돌 중이지 않은 상태 Enter
		//	{
		//		if (_left->IsTriiger())
		//		{
		//			_left->OnTriggerStay(_right);
		//		}
		//		else
		//		{
		//			_left->OnCollisionStay(_right);
		//		}
		//			

		//		if (_right->IsTriiger())
		//		{
		//			_right->OnTriggerStay(_left);
		//		}	
		//		else
		//		{
		//			_right->OnCollisionStay(_left);
		//		}
		//			
		//	}
		//}
		//else // 충돌하지 않은상태
		//{
		//	// 충돌 중인상태 Exit
		//	if (iter->second == true)
		//	{
		//		if (_left->IsTriiger())
		//			_left->OnTriggerExit(_right);
		//		else
		//			_left->OnCollisionExit(_right);

		//		if (_right->IsTriiger())
		//			_right->OnTriggerExit(_left);
		//		else
		//			_right->OnCollisionExit(_left);

		//		iter->second = false;
		//	}
		//}
	//}

	//bool CollisionSystem::Intersect(const std::shared_ptr<Collider2D>& _left, const std::shared_ptr<Collider2D>& _right)
	//{
		//// Rect vs Rect 
		//// 0 --- 1
		//// |     |
		//// 3 --- 2
		//float3 arrLocalPos[4] =
		//{
		//	float3{-0.5f, 0.5f, 0.0f}
		//	,float3{0.5f, 0.5f, 0.0f}
		//	,float3{0.5f, -0.5f, 0.0f}
		//	,float3{-0.5f, -0.5f, 0.0f}
		//};

		//const auto& leftTr = _left->GetOwner()->GetComponent<Com_Transform>();
		//const auto& rightTr = _right->GetOwner()->GetComponent<Com_Transform>();
		//if (false == (leftTr && rightTr))
		//	return false;


		//MATRIX leftMat = leftTr->GetWorldMatrix();
		//MATRIX rightMat = rightTr->GetWorldMatrix();



		//// 분리축 벡터 4개 구하기
		//float3 Axis[4] = {};

		//float3 leftScale = float3(_left->GetSize().x, _left->GetSize().y, 1.0f);

		//MATRIX finalLeft = MATRIX::CreateScale(leftScale);
		//finalLeft *= leftMat;

		//float3 rightScale = float3(_right->GetSize().x, _right->GetSize().y, 1.0f);
		//MATRIX finalRight = MATRIX::CreateScale(rightScale);
		//finalRight *= rightMat;

		//Axis[0] = float3::Transform(arrLocalPos[1], finalLeft);
		//Axis[1] = float3::Transform(arrLocalPos[3], finalLeft);
		//Axis[2] = float3::Transform(arrLocalPos[1], finalRight);
		//Axis[3] = float3::Transform(arrLocalPos[3], finalRight);

		//Axis[0] -= float3::Transform(arrLocalPos[0], finalLeft);
		//Axis[1] -= float3::Transform(arrLocalPos[0], finalLeft);
		//Axis[2] -= float3::Transform(arrLocalPos[0], finalRight);
		//Axis[3] -= float3::Transform(arrLocalPos[0], finalRight);

		//for (int index = 0; index < 4; index++)
		//	Axis[index].z = 0.0f;

		//float3 vc = leftTr->GetWorldPosition() - rightTr->GetWorldPosition();
		//vc.z = 0.0f;

		//float3 centerDir = vc;
		//for (int index = 0; index < 4; index++)
		//{
		//	float3 vA = Axis[index];
		//	//vA.Normalize();

		//	float projDist = 0.0f;
		//	for (int employee = 0; employee < 4; employee++)
		//	{
		//		projDist += fabsf(Axis[employee].Dot(vA) / 2.0f);
		//	}

		//	if (projDist < fabsf(centerDir.Dot(vA)))
		//	{
		//		return false;
		//	}
		//}


	//	return true;
	//}



	void CollisionSystem::SetCollisionMask(uint _layerA, uint _layerB, bool _isEnable)
	{
		if (_layerA < g_maxLayer && _layerB < g_maxLayer)
		{
			m_collisionMask[_layerA][_layerB] = _isEnable;
			m_collisionMask[_layerB][_layerA] = _isEnable;
		}
	}

	void CollisionSystem::SetRaycastMask(uint _layerA, uint _layerB, bool _isEnable)
	{
		if (_layerA < g_maxLayer && _layerB < g_maxLayer)
		{
			m_raycastMask[_layerA][_layerB] = _isEnable;
			m_raycastMask[_layerB][_layerA] = _isEnable;
		}
	}

}
