
#include "Com_Rigidbody_Dynamic.h"

#include <Engine/Game/Collision/PhysXInstance.h>

#include <Engine/Game/GameObject.h>
#include <Engine/Game/Component/Transform.h>

#include <Engine/DefaultSettingVar.h>

namespace core
{
	Com_Rigidbody_Dynamic::Com_Rigidbody_Dynamic()
		: Rigidbody(Com_Rigidbody_Dynamic::s_static_type_name)
		, m_isMassMode {false}
		, m_densityOrMass {g_defaultDensity}
	{
	}

	Com_Rigidbody_Dynamic::~Com_Rigidbody_Dynamic()
	{
	}

	void Com_Rigidbody_Dynamic::fixed_update()
	{
		if (IsShapesModified())
		{
			SetDensityOrMass();
			SetShapesModified(false);
		}
	}

	physx::PxRigidActor* Com_Rigidbody_Dynamic::CreateRigidbody()
	{
		s_ptr<GameObject> owner = get_owner();
		ASSERT_DEBUG(owner, "owner gameobject가 없습니다.");

		s_ptr<Transform> tr = owner->get_component<Transform>();

		physx::PxTransform pxTr{};
		pxTr.p = tr->get_world_position();
		pxTr.q = tr->get_local_rotation();

		return PhysXInstance::get_inst().GetPhysX().createRigidDynamic(pxTr);
	}


	void Com_Rigidbody_Dynamic::AddForce(const float3& _force)
	{
		physx::PxRigidDynamic* rigidActor = static_cast<physx::PxRigidDynamic*>(GetPxActor());
		if (rigidActor)
		{
			rigidActor->addForce(_force, physx::PxForceMode::eACCELERATION);
			//Transform* transform = GetOwner()->get_component<Transform>();
			//physx::PxRigidDynamic*Ext::addForceAtPos(*rigidActor, PhysXConverter::ToPxVec3(force), PhysXConverter::ToPxVec3(transform->get_world_position()));
		}
	}

	void Com_Rigidbody_Dynamic::ClearForce()
	{
		physx::PxRigidDynamic* rigidActor = static_cast<physx::PxRigidDynamic*>(GetPxActor());

		if (rigidActor)
		{
			rigidActor->clearForce();
		}
	}






	void Com_Rigidbody_Dynamic::SetVelocity(const float3& _velocity)
	{
		physx::PxRigidDynamic* dynamicRigid = static_cast<physx::PxRigidDynamic*>(GetPxActor());
		if (dynamicRigid)
		{
			dynamicRigid->setLinearVelocity(_velocity);
		}
	}

	float3 Com_Rigidbody_Dynamic::GetVelocity()
	{
		float3 retVec{};

		physx::PxRigidDynamic* dynamicRigid = static_cast<physx::PxRigidDynamic*>(GetPxActor());
		if (dynamicRigid)
		{
			retVec = dynamicRigid->getLinearVelocity();
		}

		return retVec;
	}
	void Com_Rigidbody_Dynamic::SetLimitVelocity(float _maxVelocity)
	{
		physx::PxRigidDynamic* dynamicRigid = static_cast<physx::PxRigidDynamic*>(GetPxActor());
		ASSERT_DEBUG(dynamicRigid, "PxRigidDynamic 인스턴스가 없음.");
		if (dynamicRigid)
		{
			dynamicRigid->setMaxLinearVelocity(_maxVelocity);
		}
	}

	float Com_Rigidbody_Dynamic::GetLimitVelocity()
	{
		float ret = 0.f;

		physx::PxRigidDynamic* dynamicRigid = static_cast<physx::PxRigidDynamic*>(GetPxActor());
		if (dynamicRigid)
		{
			ret = dynamicRigid->getMaxLinearVelocity();
		}

		return ret;
	}

	void Com_Rigidbody_Dynamic::SetKinematic(bool _enable)
	{
		physx::PxRigidDynamic* dynamicRigid = static_cast<physx::PxRigidDynamic*>(GetPxActor());
		if (dynamicRigid)
		{
			dynamicRigid->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, _enable);
		}
	}

	void Com_Rigidbody_Dynamic::SetMass(float _mass)
	{
		m_isMassMode = true;
		m_densityOrMass = _mass;

		physx::PxRigidDynamic* dynamicRigid = static_cast<physx::PxRigidDynamic*>(GetPxActor());
		if (dynamicRigid)
		{
			physx::PxRigidBodyExt::setMassAndUpdateInertia(*dynamicRigid, _mass);
		}
	}

	float Com_Rigidbody_Dynamic::GetMass() const
	{
		float ret = 0.f;

		const physx::PxRigidDynamic* dynamicRigid = static_cast<const physx::PxRigidDynamic*>(GetPxActor());
		if (dynamicRigid)
		{
			ret = dynamicRigid->getMass();
		}
		return ret;
	}

	void Com_Rigidbody_Dynamic::SetDensity(float _density)
	{
		m_isMassMode = false;
		m_densityOrMass = _density;

		physx::PxRigidDynamic* dynamicRigid = static_cast<physx::PxRigidDynamic*>(GetPxActor());
		if (dynamicRigid)
		{
			physx::PxRigidBodyExt::updateMassAndInertia(*dynamicRigid, _density);
		}
	}

	void Com_Rigidbody_Dynamic::SetRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::Enum _flag, bool _enable)
	{
		physx::PxRigidDynamic* rigidActor = static_cast<physx::PxRigidDynamic*>(GetPxActor());
		if (rigidActor)
		{
			rigidActor->setRigidDynamicLockFlag(_flag, _enable);
		}
	}

	bool Com_Rigidbody_Dynamic::GetRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::Enum _flag) const
	{
		const physx::PxRigidDynamic* rigidActor = static_cast<const physx::PxRigidDynamic*>(GetPxActor());
		if (rigidActor)
		{
			physx::PxRigidDynamicLockFlags flags = rigidActor->getRigidDynamicLockFlags();
			if (flags & _flag)
			{
				return true;
			}
		}
		return false;
	}

	void Com_Rigidbody_Dynamic::SetRigidDynamicLockFlags(physx::PxRigidDynamicLockFlags _flags)
	{
		physx::PxRigidDynamic* rigidActor = static_cast<physx::PxRigidDynamic*>(GetPxActor());
		if (rigidActor)
		{
			rigidActor->setRigidDynamicLockFlags(_flags);
		}
	}

	physx::PxRigidDynamicLockFlags Com_Rigidbody_Dynamic::GetLockFlags() const
	{
		physx::PxRigidDynamicLockFlags ret{};

		const physx::PxRigidDynamic* rigidActor = static_cast<const physx::PxRigidDynamic*>(GetPxActor());
		if (rigidActor)
		{
			rigidActor->getRigidDynamicLockFlags();
		}
		return ret;
	}


}

