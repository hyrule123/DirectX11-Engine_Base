#include "Engine/Game/Component/Collider/Collider3D.h"

#include "Engine/Game/GameObject.h"
#include "Engine/Game/Scene.h"
#include "Engine/Game/Component/Transform.h"
#include "Engine/Game/Component/Rigidbody/Com_Rigidbody_Static.h"

#include "Engine/Game/Collision/CollisionSystem.h"
#include "Engine/Game/Collision/Collision3D.h"

#include <PhysX/PxPhysicsAPI.h>

namespace core
{
	Collider3D::Collider3D(const std::string_view key, eCollider3DType _col3dType)
		: Collider(key, eDimensionType::_3D)
		, m_colliderType{ _col3dType }
		, m_isSyncScaleToTransform(true)
	{
	}

	Collider3D::~Collider3D()
	{
	}

	void Collider3D::init()
	{
		Collider::init();
	}

	void Collider3D::Awake()
	{
		Collider::Awake();

		CollisionSystem* colsys = GetCollisionSystem();
		if (colsys)
		{
			m_col3dMgr = colsys->GetCollision3D();
		}
	}



	//bool Collider3D::isOverlapping(Collider3D* other, float3* outDistance)
	//{
		//physx::PxVec3 direction{};
		//physx::PxF32  depth{};

		//Collider3D* otherCollider = other->get_component<Collider3D>();
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




	//void Collider3D::UpdateMatrix()
	//{
	//	//이동은 크기에 영향을 받으므로 우선 크기 정보를 반영해서 MATRIX를 만들어줘야 한다.
	//	_localMatrix = MATRIX::CreateScale(m_offsetScale);
	//	_localMatrix *= MATRIX::CreateTranslation(m_offsetPosition);

	//	//트랜스폼의 크기 정보는 제거
	//	const float3 objectScale = GetOwner()->get_component<Transform>()->get_world_scale();
	//	const MATRIX objectScaleInvMatrix = MATRIX::CreateScale(objectScale).Invert();

	//	const MATRIX  objectWorldMatrix = GetOwner()->get_component<Transform>()->get_world_matrix();
	//	// 충돌체 상대행렬 * 오브젝트 월드 크기 역행렬 * 오브젝트 월드 행렬(크기 * 회전 * 이동)
	//	//트랜스폼의 Scale은 반영하지 않는다
	//	_worldMatrix = _localMatrix * objectScaleInvMatrix * objectWorldMatrix;
	//}
} // namespace core
