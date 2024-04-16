#include "Engine/Game/Component/Collider/iCollider3D.h"

#include "Engine/Game/GameObject.h"
#include "Engine/Game/iScene.h"
#include "Engine/Game/Component/Transform/Com_Transform.h"

#include "Engine/Game/Collision/CollisionSystem.h"
#include "Engine/Game/Collision/Collision3D.h"


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

		physx::PxActor* actor = CreatePxActor();
		ASSERT(actor, "Actor 생성 실패");
		SetPxActor(actor);
	}

	void iCollider3D::Awake()
	{
		iCollider::Awake();

		m_collision3D = GetCollisionSystem()->GetCollision3D();
	}

	void iCollider3D::OnEnable()
	{
		iCollider::OnEnable();

		Collision3D* col3d = GetCollision3D();
		if (col3d)
		{
			bool result = col3d->AddPxActor(m_pxActor);
			ASSERT_DEBUG(result, "PxActor 삽입 실패");
		}
	}

	void iCollider3D::OnDisable()
	{
		Collision3D* col3d = GetCollision3D();
		if (col3d)
		{
			col3d->RemovePxActor(m_pxActor);
		}
	}

	void iCollider3D::OnDestroy()
	{
		PX_RELEASE(m_pxActor);
	}


	void iCollider3D::SetPxActor(physx::PxActor* const _pxActor)
	{
		if (nullptr == _pxActor)
		{
			ASSERT_DEBUG(false, "PxActor가 nullptr 입니다.");
			return;
		}

		if (m_pxActor)
		{
			//씬에 들어가있을 경우 씬으로부터 제거
			physx::PxScene* scene = m_pxActor->getScene();
			if (scene)
			{
				scene->removeActor(*_pxActor);
			}
			
			m_pxActor->release();
			m_pxActor = nullptr;
		}

		m_pxActor = _pxActor;
		m_pxActor->userData = this;
	}

	void iCollider3D::SceneChanged()
	{
		//if (m_pxShape)
		//{
		//	physx::PxActor* actor = m_pxShape->getActor();
		//	actor->userData = nullptr;
		//	actor->release();
		//	m_pxShape = nullptr;
		//}
		//SetType(GetCollider3DType(), m_isStatic);
	}

	void iCollider3D::EnableGravity(bool enable)
	{
		if (m_pxActor)
		{
			auto flags = m_pxActor->getActorFlags();
			if (enable)
			{
				flags &= ~physx::PxActorFlag::eDISABLE_GRAVITY;
			}
			else
			{
				flags |= physx::PxActorFlag::eDISABLE_GRAVITY;
			}

			m_pxActor->setActorFlags(flags);
		}
	}

	bool iCollider3D::IsGravityEnabled() const
	{
		if (m_pxActor)
		{
			auto flags = m_pxActor->getActorFlags();

			if (!(physx::PxActorFlag::eDISABLE_GRAVITY & flags))
			{
				return true;
			}
		}

		return false;
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
