#include "Engine/Scene/Component/Collider/iCollider3D.h"

#include "Engine/Scene/GameObject.h"
#include "Engine/Scene/iScene.h"
#include "Engine/Scene/Component/Transform/Com_Transform.h"
#include "Engine/Scene/Component/Rigidbody/Com_Rigidbody_Static.h"

#include "Engine/Scene/Collision/CollisionSystem.h"
#include "Engine/Scene/Collision/Collision3D.h"

#include <PhysX/PxPhysicsAPI.h>

namespace ehw
{
	iCollider3D::iCollider3D(eCollider3DType _col3dType)
		: iCollider{eDimensionType::_3D}
		, m_colliderType{ _col3dType }
		, m_isSyncScaleToTransform(true)
	{
	}

	iCollider3D::~iCollider3D()
	{
	}

	void iCollider3D::Init()
	{
		iCollider::Init();
	}

	void iCollider3D::Awake()
	{
		iCollider::Awake();

		CollisionSystem* colsys = GetCollisionSystem();
		if (colsys)
		{
			m_col3dMgr = colsys->GetCollision3D();
		}
	}



	//bool iCollider3D::isOverlapping(iCollider3D* other, float3* outDistance)
	//{
		//physx::PxVec3 direction{};
		//physx::PxF32  depth{};

		//iCollider3D* otherCollider = other->GetComponent<iCollider3D>();
		//const bool		  isPenetrating = physx::PxGeometryQuery::computePenetration(direction, depth,
		//	m_shape->getGeometry(),
		//	m_shape->getActor()->getGlobalPose(),
		//	otherCollider->getShape()->getGeometry(),
		//	otherCollider->getShape()->getActor()->getGlobalPose());

		//if (outDistance && isPenetrating)
		//{
		//	const physx::PxVec3 result = direction * depth;
		//	*outDistance = PhysXConverter::Tofloat3(result);
		//}

		//return isPenetrating;
	//}




	//void iCollider3D::UpdateMatrix()
	//{
	//	//이동은 크기에 영향을 받으므로 우선 크기 정보를 반영해서 MATRIX를 만들어줘야 한다.
	//	_localMatrix = MATRIX::CreateScale(m_offsetScale);
	//	_localMatrix *= MATRIX::CreateTranslation(m_offsetPosition);

	//	//트랜스폼의 크기 정보는 제거
	//	const float3 objectScale = GetOwner()->GetComponent<Com_Transform>()->GetWorldScale();
	//	const MATRIX objectScaleInvMatrix = MATRIX::CreateScale(objectScale).Invert();

	//	const MATRIX  objectWorldMatrix = GetOwner()->GetComponent<Com_Transform>()->GetWorldMatrix();
	//	// 충돌체 상대행렬 * 오브젝트 월드 크기 역행렬 * 오브젝트 월드 행렬(크기 * 회전 * 이동)
	//	//트랜스폼의 Scale은 반영하지 않는다
	//	_worldMatrix = _localMatrix * objectScaleInvMatrix * objectWorldMatrix;
	//}
} // namespace ehw
