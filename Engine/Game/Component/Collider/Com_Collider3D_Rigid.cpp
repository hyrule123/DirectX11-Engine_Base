#include "Engine/Game/Component/Collider/Com_Collider3D_Rigid.h"


#include "Engine/Game/Collision/PhysXInstance.h"
#include "Engine/Game/Collision/Collision3D.h"
#include "Engine/Game/Component/Transform/Com_Transform.h"

#include "Engine/Game/iScene.h"
#include "Engine/Game/GameObject.h"

namespace ehw
{
	Com_Collider3D_Rigid::Com_Collider3D_Rigid()
		: iCollider3D(eCollider3DType::Rigid)
		, m_isDynamic(false)
		, m_freezeRotationFlag{}
		, m_prevLayer{ UINT32_MAX }
		, m_pxShapes{}
	{
	}
	Com_Collider3D_Rigid::Com_Collider3D_Rigid(eCollider3D_Shape _shape)
		: iCollider3D(eCollider3DType::Rigid)
		, m_isDynamic(false)
	{
	}
	Com_Collider3D_Rigid::~Com_Collider3D_Rigid()
	{
	}

	//void Com_Collider3D_Rigid::OnEnable()
	//{
	//	iCollider3D::OnEnable();

	//	physx::PxRigidActor* rigidActor = GetPxActor()->is<physx::PxRigidActor>();
	//	ASSERT_DEBUG(rigidActor, "PxRigActor 인스턴스가 없음.");



	//	for (size_t i = 0; i < m_pxShapes.size(); ++i)
	//	{
	//		rigidActor->attachShape(*(m_pxShapes[i]));
	//	}
	//}

	//void Com_Collider3D_Rigid::OnDisable()
	//{
	//	iCollider3D::OnDisable();

	//	physx::PxRigidActor* rigidActor = GetPxActor()->is<physx::PxRigidActor>();
	//	if (rigidActor)
	//	{
	//		for (size_t i = 0; i < m_pxShapes.size(); ++i)
	//		{
	//			rigidActor->detachShape(*(m_pxShapes[i]));
	//		}
	//	}
	//}

	void Com_Collider3D_Rigid::Update()
	{
		//레이어 번호 변경되었을 시 Shape의 Filter Data를 전부 변경한다.
		const uint32 curLayer = GetOwner()->GetLayer();
		if (curLayer != m_prevLayer)
		{
			m_prevLayer = curLayer;
			physx::PxFilterData collFilter = GetCollision3D()->GetCollisionFilterData(curLayer);
			physx::PxFilterData rayFilter = GetCollision3D()->GetRaycastFilterData(curLayer);

			for (size_t i = 0; i < m_pxShapes.size(); ++i)
			{
				m_pxShapes[i]->setSimulationFilterData(collFilter);
				m_pxShapes[i]->setQueryFilterData(rayFilter);
			}
		}
	}

	void Com_Collider3D_Rigid::OnDestroy()
	{
		iCollider3D::OnDestroy();

		for (size_t i = 0; i < m_pxShapes.size(); ++i)
		{
			m_pxShapes[i]->release();
		}
		m_pxShapes.clear();
	}


	//void Com_Collider3D_Rigid::SyncToPhysX()
	//{
	//	Com_Transform* tr = GetTransform();
	//	if (false == tr->IsTransformUpdated())
	//	{
	//		return;
	//	}

	//	physx::PxRigidActor* rigidActor = GetPxActor()->is<physx::PxRigidActor>();

	//	if (nullptr == rigidActor)
	//	{
	//		ASSERT_DEBUG(nullptr, "rigidActor 생성되지 않음");
	//	}

	//	physx::PxTransformT<float> transform{};
	//	transform.p = tr->GetWorldPosition();
	//	transform.q = tr->GetLocalRotation();

	//	rigidActor->setGlobalPose(transform);
	//}

	//void Com_Collider3D_Rigid::FetchFromPhysX()
	//{
	//	Com_Transform* tr = GetTransform();

	//	physx::PxRigidActor* rigidActor = GetPxActor()->is<physx::PxRigidActor>();
	//	ASSERT_DEBUG(rigidActor, "rigidActor가 nullptr");

	//	physx::PxTransform worldTransform = rigidActor->getGlobalPose();

	//	if (m_isDynamic)
	//	{
	//		tr->SetWorldPosition(worldTransform.p);
	//		tr->SetLocalRotation(worldTransform.q);

	//		//MATRIX before = MATRIX::CreateScale(m_offsetScale).Invert() * _worldMatrix;
	//		//MATRIX after = PhysXConverter::ToMATRIX(physx::PxMat44(worldTransform));
	//		//MATRIX diff = before.Invert() * after;

	//		//Quaternion diffQuat = Quaternion::CreateFromRotationMatrix(diff);

	//		//float3 diffPos = diff.Translation();

	//		//Com_Transform* tr = GetOwner()->GetComponent<Com_Transform>();
	//		//tr->SetWorldRotation(diffQuat);
	//		//tr->SetWorldPosition(diffPos);
	//		////GetOwner()->GetComponent<Com_Transform>()->FetchPhysX(diffQuat, diffPos);
	//	}
	//}

	void Com_Collider3D_Rigid::SetDynamic(bool _isDynamic)
	{
		if (_isDynamic == m_isDynamic)
		{
			return;
		}

		m_isDynamic = _isDynamic;
		physx::PxRigidActor* actor = static_cast<physx::PxRigidActor*>(CreatePxActor());

		for (size_t i = 0; i < m_pxShapes.size(); ++i)
		{
			actor->attachShape(*m_pxShapes[i]);
		}
		
		SetPxActor(actor);
	}

	void Com_Collider3D_Rigid::AttachShape(physx::PxShape* const _pxShape)
	{
		if (nullptr == _pxShape)
		{
			return;
		}	

		m_pxShapes.push_back(_pxShape);

		physx::PxRigidActor* rigidActor = GetPxActor()->is<physx::PxRigidActor>();
		if (rigidActor)
		{
			bool result = rigidActor->attachShape(*(m_pxShapes.back()));
			ASSERT_DEBUG(result, "attach 실패");
		}
	}
	void Com_Collider3D_Rigid::DetachShape(physx::PxShape* const _pxShape)
	{
		for (auto iter = m_pxShapes.begin(); iter != m_pxShapes.end(); ++iter)
		{
			if (_pxShape == (*iter))
			{
				physx::PxRigidActor* rigidActor = GetPxActor()->is<physx::PxRigidActor>();
				if (rigidActor)
				{
					rigidActor->detachShape(*_pxShape);
				}

				m_pxShapes.erase(iter);
				break;
			}
		}
	}
	physx::PxActor* Com_Collider3D_Rigid::CreatePxActor()
	{
		physx::PxActor* ret{};

		Com_Transform* tr = GetTransform();

		physx::PxTransform pxTr{};
		pxTr.p = tr->GetWorldPosition();
		pxTr.q = tr->GetLocalRotation();

		if (m_isDynamic)
		{
			ret = PhysXInstance::GetInst().createRigidDynamic(pxTr);
		}
		else
		{
			ret = PhysXInstance::GetInst().createRigidStatic(pxTr);
		}

		ASSERT_DEBUG(ret, "Actor가 생성되지 않았습니다.");

		return ret;
	}

	void Com_Collider3D_Rigid::SetFreezeRotation(EnumFlags<eFreezeRotationFlag> _flag)
	{
		m_freezeRotationFlag = _flag;

		physx::PxRigidDynamic* rigidActor = GetPxActor()->is<physx::PxRigidDynamic>();
		if (rigidActor == nullptr)
		{
			return;
		}

		if (rigidActor)
		{
			physx::PxRigidDynamicLockFlags pxFlag{};

			if (m_freezeRotationFlag & eFreezeRotationFlag::ROTATION_X)
			{
				pxFlag |= physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X;
			}
			if (m_freezeRotationFlag & eFreezeRotationFlag::ROTATION_Y)
			{
				pxFlag |= physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y;
			}
			if (m_freezeRotationFlag & eFreezeRotationFlag::ROTATION_Z)
			{
				pxFlag |= physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z;
			}

			rigidActor->setRigidDynamicLockFlags(pxFlag);
			rigidActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_X, true);
			rigidActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, true);
		}
	}


	void Com_Collider3D_Rigid::AddForce(const float3& _force)
	{
		physx::PxRigidDynamic* rigidActor = GetPxActor()->is<physx::PxRigidDynamic>();

		if (rigidActor)
		{
			rigidActor->addForce(_force, physx::PxForceMode::eACCELERATION);
			//Com_Transform* transform = GetOwner()->GetComponent<Com_Transform>();
			//physx::PxRigidBodyExt::addForceAtPos(*rigidActor, PhysXConverter::ToPxVec3(force), PhysXConverter::ToPxVec3(transform->GetWorldPosition()));
		}
	}

	void Com_Collider3D_Rigid::ClearForce()
	{
		if (false == m_isDynamic)
		{
			DEBUG_LOG_W(L"Dynamic Actor가 아닌 충돌체에 ClearForce를 호출했습니다.");
			return;
		}

		physx::PxRigidDynamic* rigidActor = GetPxActor()->is<physx::PxRigidDynamic>();

		if (rigidActor)
		{
			rigidActor->clearForce();
			//Com_Transform* transform = GetOwner()->GetComponent<Com_Transform>();
			//physx::PxRigidBodyExt::addForceAtPos(*rigidActor, PhysXConverter::ToPxVec3(force), PhysXConverter::ToPxVec3(transform->GetWorldPosition()));
		}
	}

	void Com_Collider3D_Rigid::SetTrigger(bool _isTrigger)
	{
		if (m_isTrigger == _isTrigger)
		{
			return;
		}

		m_isTrigger = _isTrigger;

		for (size_t i = 0; i < m_pxShapes.size(); ++i)
		{
			physx::PxShapeFlags flags = m_pxShapes[i]->getFlags();

			if (m_isTrigger)
			{
				flags &= ~physx::PxShapeFlag::eSIMULATION_SHAPE;
				flags |= physx::PxShapeFlag::eTRIGGER_SHAPE;
			}
			else
			{
				flags &= ~physx::PxShapeFlag::eTRIGGER_SHAPE;
				flags |= physx::PxShapeFlag::eSIMULATION_SHAPE;
			}

			m_pxShapes[i]->setFlags(flags);
		}
	}


	void Com_Collider3D_Rigid::SetVelocity(const float3& _velocity)
	{
		if (false == m_isDynamic)
		{
			return;
		}
		
		physx::PxRigidDynamic* dynamicRigid = GetPxActor()->is<physx::PxRigidDynamic>();
		if (dynamicRigid)
		{
			dynamicRigid->setLinearVelocity(_velocity);
		}
	}

	float3 Com_Collider3D_Rigid::GetVelocity()
	{
		float3 retVec{};

		if (m_isDynamic)
		{
			physx::PxRigidDynamic* dynamicRigid = GetPxActor()->is<physx::PxRigidDynamic>();
			if (dynamicRigid)
			{
				retVec = dynamicRigid->getLinearVelocity();
			}
		}

		return retVec;
	}
	void Com_Collider3D_Rigid::SetLimitVelocity(float _maxVelocity)
	{
		if (false == m_isDynamic)
		{
			return;
		}

		physx::PxRigidDynamic* dynamicRigid = GetPxActor()->is<physx::PxRigidDynamic>();
		ASSERT_DEBUG(dynamicRigid, "PxRigidDynamic 인스턴스가 없음.");
		if (dynamicRigid)
		{
			dynamicRigid->setMaxLinearVelocity(_maxVelocity);
		}
	}

	float Com_Collider3D_Rigid::GetLimitVelocity() const
	{
		float ret = 0.f;
		if (false == m_isDynamic)
		{
			return ret;
		}
		const physx::PxRigidDynamic* dynamicRigid = GetPxActor()->is<physx::PxRigidDynamic>();
		ASSERT_DEBUG(dynamicRigid, "PxRigidDynamic 인스턴스가 없음.");
		if (dynamicRigid)
		{
			ret = dynamicRigid->getMaxLinearVelocity();
		}

		return ret;
	}

	void Com_Collider3D_Rigid::SetKinematic(bool enable)
	{
		physx::PxRigidBody* rigid = GetPxActor()->is<physx::PxRigidBody>();
		ASSERT_DEBUG(rigid, "PxRigidBody 인스턴스가 없음.");
		if (rigid)
		{
			rigid->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, enable);
		}
	}

	void Com_Collider3D_Rigid::SetMass(float _mass)
	{
		physx::PxRigidBody* rigidBody = GetPxActor()->is<physx::PxRigidBody>();
		if (rigidBody)
		{
			rigidBody->setMass(_mass);
		}
	}
}
