#include "Engine/Game/Component/Rigidbody/iRigidbody.h"

#include "Engine/Game/GameObject.h"
#include "Engine/Game/iScene.h"
#include "Engine/Game/Collision/CollisionSystem.h"
#include "Engine/Game/Collision/Collision3D.h"



namespace ehw
{
	iRigidbody::iRigidbody()
		: m_rigidActor{}
		, m_ShapesModified{true}
	{
	}

	iRigidbody::~iRigidbody()
	{
	}

	void iRigidbody::Init()
	{
		m_rigidActor.Set(CreateRigidbody());
		ASSERT_DEBUG(m_rigidActor, "PxActor 생성되지 않음.");
		
		if (m_rigidActor)
		{
			m_rigidActor->userData = this;
		}
	}
	void iRigidbody::OnEnable()
	{
		CollisionSystem* colsys = GetOwner()->GetScene()->GetCollisionSystem();
		if (colsys)
		{
			Collision3D* col3d = colsys->GetCollision3D();
			if (col3d)
			{
				bool ret = col3d->AddPxActor(m_rigidActor.Get());
				ASSERT_DEBUG(ret, "AddPxActor 실패.");
			}
		}
	}
	void iRigidbody::OnDisable()
	{
		physx::PxScene* scene = m_rigidActor->getScene();
		if (scene)
		{
			scene->removeActor(*(static_cast<physx::PxActor*>(m_rigidActor.Get())));
		}
	}

	void iRigidbody::OnDestroy()
	{
		m_rigidActor.Reset();
	}


	bool iRigidbody::AttachColliderShape(physx::PxShape* _pxShape)
	{
		bool ret = false;
		if (m_rigidActor)
		{
			ret = m_rigidActor->attachShape(*_pxShape);
			m_ShapesModified = true;
		}
		return ret;
	}
	void iRigidbody::DetachColliderShape(physx::PxShape* _pxShape)
	{
		if (m_rigidActor)
		{
			m_rigidActor->detachShape(*_pxShape);
			m_ShapesModified = true;
		}
	}
	void iRigidbody::UpdateGlobalPose()
	{
		Com_Transform* tr = GetOwner()->Transform();
		if (false == tr->IsTransformUpdated())
		{
			return;
		}

		physx::PxTransformT<float> transform{};
		transform.p = tr->GetWorldPosition();
		transform.q = tr->GetLocalRotation();

		m_rigidActor->setGlobalPose(transform);
	}
	void iRigidbody::EnableGravity(bool enable)
	{
		if (m_rigidActor)
		{
			auto flags = m_rigidActor->getActorFlags();
			if (enable)
			{
				flags &= ~physx::PxActorFlag::eDISABLE_GRAVITY;
			}
			else
			{
				flags |= physx::PxActorFlag::eDISABLE_GRAVITY;
			}

			m_rigidActor->setActorFlags(flags);
		}
	}

	bool iRigidbody::IsGravityEnabled() const
	{
		if (m_rigidActor)
		{
			auto flags = m_rigidActor->getActorFlags();

			if (!(physx::PxActorFlag::eDISABLE_GRAVITY & flags))
			{
				return true;
			}
		}

		return false;
	}
}
