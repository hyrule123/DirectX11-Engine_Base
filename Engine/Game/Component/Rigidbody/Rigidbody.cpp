#include "Engine/Game/Component/Rigidbody/Rigidbody.h"

#include "Engine/Game/GameObject.h"
#include "Engine/Game/Component/Transform.h"
#include "Engine/Game/Scene.h"
#include "Engine/Game/Collision/CollisionSystem.h"
#include "Engine/Game/Collision/Collision3D.h"

namespace ehw
{
	Rigidbody::Rigidbody(const std::string_view key)
		: Component(key)
		, m_rigidActor{}
		, m_ShapesModified{true}
	{
	}

	Rigidbody::~Rigidbody()
	{
	}

	void Rigidbody::init()
	{
		m_rigidActor.Set(CreateRigidbody());
		ASSERT_DEBUG(m_rigidActor, "PxActor 생성되지 않음.");
		
		if (m_rigidActor)
		{
			m_rigidActor->userData = this;
		}
	}
	void Rigidbody::OnEnable()
	{
		CollisionSystem* colsys = gameObject()->scene()->GetCollisionSystem();
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
	void Rigidbody::OnDisable()
	{
		physx::PxScene* scene = m_rigidActor->getScene();
		if (scene)
		{
			scene->removeActor(*(static_cast<physx::PxActor*>(m_rigidActor.Get())));
		}
	}

	void Rigidbody::OnDestroy()
	{
		m_rigidActor.Reset();
	}


	bool Rigidbody::AttachColliderShape(physx::PxShape* _pxShape)
	{
		bool ret = false;
		if (m_rigidActor)
		{
			ret = m_rigidActor->attachShape(*_pxShape);
			m_ShapesModified = true;
		}
		return ret;
	}
	void Rigidbody::DetachColliderShape(physx::PxShape* _pxShape)
	{
		if (m_rigidActor)
		{
			m_rigidActor->detachShape(*_pxShape);
			m_ShapesModified = true;
		}
	}
	void Rigidbody::SyncToPhysXGlobalPose()
	{
		Transform* tr = gameObject()->transform();
		if (false == tr->is_transform_updated())
		{
			return;
		}

		physx::PxTransformT<float> transform{};
		transform.p = tr->get_world_position();
		transform.q = tr->get_local_rotation();

		m_rigidActor->setGlobalPose(transform);
	}

	void Rigidbody::FetchFromPhysXGlobalPose(const physx::PxTransform& _pxTransform)
	{
		Transform* tr = gameObject()->transform();

		tr->set_world_position(_pxTransform.p);
		tr->set_local_rotation(_pxTransform.q);
	}


	void Rigidbody::EnableGravity(bool enable)
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

	bool Rigidbody::IsGravityEnabled() const
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
