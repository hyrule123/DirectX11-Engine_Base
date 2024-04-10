#include "Engine/Game/Component/Collider/iCollider3D.h"

#include "Engine/Game/GameObject.h"
#include "Engine/Game/iScene.h"
#include "Engine/Game/Component/Transform/Com_Transform.h"

#include "Engine/Game/Collision/CollisionSystem.h"
#include "Engine/Game/Collision/PhysXConverter.h"



#include <PhysX/PxPhysicsAPI.h>

namespace ehw
{
	iCollider3D::iCollider3D(eCollider3DType _col3dType)
		: iCollider{eDimensionType::_3D}
		, m_colliderType{ _col3dType }
		, m_myTransform{nullptr}
		, m_isSyncScaleToTransform(true)
		, m_collisionCount{ 0 }
		, m_isGravityEnable(true)
		, m_mass(0.6f)
		, m_maxVelocity(40.f)
		, m_isKinematic(false)
	{
	}

	iCollider3D::~iCollider3D()
	{
	}

	void iCollider3D::Init()
	{
		//Transform은 Init에서도 가져올 수 있음
		m_myTransform = GetOwner()->Transform();

		m_pxActor = CreateActor();
		ASSERT(m_pxActor, "Actor 생성 실패");
		m_pxActor->userData = this;

		m_collision3D = GetOwner()->GetScene()->GetCollisionSystem()->GetCollision3D();
	}

	void iCollider3D::Awake()
	{
		EnableGravity(m_isGravityEnable);
	}

	void iCollider3D::OnDestroy()
	{
		PX_RELEASE(m_pxActor);
	}

	void iCollider3D::OnCollisionEnter(iCollider3D* other, const float3& collisionPosition)
	{
		++m_collisionCount;

		const auto& scripts = GetOwner()->GetScripts();
		for (auto& script : scripts)
		{
			script->OnCollisionEnter(other, collisionPosition);
		}
	}

	void iCollider3D::OnCollisionStay(iCollider3D* const other, const float3& collisionPosition)
	{
		const auto& scripts = GetOwner()->GetScripts();
		for (auto& script : scripts)
		{
			script->OnCollisionStay(other, collisionPosition);
		}
	}

	void iCollider3D::OnCollisionExit(iCollider3D* const other)
	{
		--m_collisionCount;
		const auto& scripts = GetOwner()->GetScripts();
		for (auto& script : scripts)
		{
			script->OnCollisionExit(other);
		}
	}

	void iCollider3D::OnTriggerEnter(iCollider3D* const other)
	{
		const auto& scripts = GetOwner()->GetScripts();
		for (auto& script : scripts)
		{
			script->OnTriggerEnter(other);
		}

		//_wireFrameData.isTrigger = true;
	}

	void iCollider3D::OnTriggerStay(iCollider3D* const other)
	{
		const auto& scripts = GetOwner()->GetScripts();
		for (auto& script : scripts)
		{
			script->OnTriggerEnter(other);
		}
	}



	void iCollider3D::OnTriggerExit(iCollider3D* const other)
	{
		const auto& scripts = GetOwner()->GetScripts();
		for (auto& script : scripts)
		{
			script->OnTriggerExit(other);
		}
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
		m_isGravityEnable = enable;

		if (m_pxActor)
		{
			auto flags = m_pxActor->getActorFlags();
			if (m_isGravityEnable)
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
