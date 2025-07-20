#include "DynamicRigidbody.h"

#include <Engine/Game/Collision/PhysXInstance.h>

#include <Engine/Game/GameObject.h>
#include <Engine/Game/Component/Transform.h>

#include <Engine/DefaultSettingVar.h>

namespace core
{
	DynamicRigidbody::DynamicRigidbody()
		: Rigidbody(DynamicRigidbody::s_static_type_name)
		, m_isMassMode {false}
		, m_densityOrMass {g_defaultDensity}
	{
	}

	DynamicRigidbody::~DynamicRigidbody()
	{
	}

	void DynamicRigidbody::fixed_update()
	{
		if (is_shapes_modified())
		{
			set_density_or_mass();
			set_shapes_modified(false);
		}
	}

	physx::PxRigidActor* DynamicRigidbody::create_rigidbody()
	{
		s_ptr<GameObject> owner = get_owner();
		ASSERT_DEBUG(owner, "owner gameobject가 없습니다.");

		s_ptr<Transform> tr = owner->get_component<Transform>();

		physx::PxTransform pxTr{};
		pxTr.p = tr->get_world_position();
		pxTr.q = tr->get_local_rotation();

		return PhysXInstance::get_inst().GetPhysX().createRigidDynamic(pxTr);
	}

	void DynamicRigidbody::set_density_or_mass()
	{
		if (m_isMassMode)
		{
			set_mass(m_densityOrMass);
		}
		else
		{
			set_density(m_densityOrMass);
		}
	}


	void DynamicRigidbody::add_force(const float3& _force)
	{
		physx::PxRigidDynamic* rigidActor = static_cast<physx::PxRigidDynamic*>(get_px_actor());
		if (rigidActor)
		{
			rigidActor->addForce(_force, physx::PxForceMode::eACCELERATION);
			//Transform* transform = GetOwner()->get_component<Transform>();
			//physx::PxRigidDynamic*Ext::addForceAtPos(*rigidActor, PhysXConverter::ToPxVec3(force), PhysXConverter::ToPxVec3(transform->get_world_position()));
		}
	}

	void DynamicRigidbody::clear_force()
	{
		physx::PxRigidDynamic* rigidActor = static_cast<physx::PxRigidDynamic*>(get_px_actor());

		if (rigidActor)
		{
			rigidActor->clearForce();
		}
	}






	void DynamicRigidbody::set_velocity(const float3& _velocity)
	{
		physx::PxRigidDynamic* dynamicRigid = static_cast<physx::PxRigidDynamic*>(get_px_actor());
		if (dynamicRigid)
		{
			dynamicRigid->setLinearVelocity(_velocity);
		}
	}

	float3 DynamicRigidbody::get_velocity()
	{
		float3 retVec{};

		physx::PxRigidDynamic* dynamicRigid = static_cast<physx::PxRigidDynamic*>(get_px_actor());
		if (dynamicRigid)
		{
			retVec = dynamicRigid->getLinearVelocity();
		}

		return retVec;
	}
	void DynamicRigidbody::set_limit_velocity(float _maxVelocity)
	{
		physx::PxRigidDynamic* dynamicRigid = static_cast<physx::PxRigidDynamic*>(get_px_actor());
		ASSERT_DEBUG(dynamicRigid, "PxRigidDynamic 인스턴스가 없음.");
		if (dynamicRigid)
		{
			dynamicRigid->setMaxLinearVelocity(_maxVelocity);
		}
	}

	float DynamicRigidbody::get_limit_velocity()
	{
		float ret = 0.f;

		physx::PxRigidDynamic* dynamicRigid = static_cast<physx::PxRigidDynamic*>(get_px_actor());
		if (dynamicRigid)
		{
			ret = dynamicRigid->getMaxLinearVelocity();
		}

		return ret;
	}

	void DynamicRigidbody::set_kinematic(bool _enable)
	{
		physx::PxRigidDynamic* dynamicRigid = static_cast<physx::PxRigidDynamic*>(get_px_actor());
		if (dynamicRigid)
		{
			dynamicRigid->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, _enable);
		}
	}

	void DynamicRigidbody::set_mass(float _mass)
	{
		m_isMassMode = true;
		m_densityOrMass = _mass;

		physx::PxRigidDynamic* dynamicRigid = static_cast<physx::PxRigidDynamic*>(get_px_actor());
		if (dynamicRigid)
		{
			physx::PxRigidBodyExt::setMassAndUpdateInertia(*dynamicRigid, _mass);
		}
	}

	float DynamicRigidbody::get_mass() const
	{
		float ret = 0.f;

		const physx::PxRigidDynamic* dynamicRigid = static_cast<const physx::PxRigidDynamic*>(get_px_actor());
		if (dynamicRigid)
		{
			ret = dynamicRigid->getMass();
		}
		return ret;
	}

	void DynamicRigidbody::set_density(float _density)
	{
		m_isMassMode = false;
		m_densityOrMass = _density;

		physx::PxRigidDynamic* dynamicRigid = static_cast<physx::PxRigidDynamic*>(get_px_actor());
		if (dynamicRigid)
		{
			physx::PxRigidBodyExt::updateMassAndInertia(*dynamicRigid, _density);
		}
	}

	void DynamicRigidbody::set_rigid_dynamic_lock_flag(physx::PxRigidDynamicLockFlag::Enum _flag, bool _enable)
	{
		physx::PxRigidDynamic* rigidActor = static_cast<physx::PxRigidDynamic*>(get_px_actor());
		if (rigidActor)
		{
			rigidActor->setRigidDynamicLockFlag(_flag, _enable);
		}
	}

	bool DynamicRigidbody::get_rigid_dynamic_lock_flag(physx::PxRigidDynamicLockFlag::Enum _flag) const
	{
		const physx::PxRigidDynamic* rigidActor = static_cast<const physx::PxRigidDynamic*>(get_px_actor());
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

	void DynamicRigidbody::set_rigid_dynamic_lock_flags(physx::PxRigidDynamicLockFlags _flags)
	{
		physx::PxRigidDynamic* rigidActor = static_cast<physx::PxRigidDynamic*>(get_px_actor());
		if (rigidActor)
		{
			rigidActor->setRigidDynamicLockFlags(_flags);
		}
	}

	physx::PxRigidDynamicLockFlags DynamicRigidbody::get_lock_flags() const
	{
		physx::PxRigidDynamicLockFlags ret{};

		const physx::PxRigidDynamic* rigidActor = static_cast<const physx::PxRigidDynamic*>(get_px_actor());
		if (rigidActor)
		{
			rigidActor->getRigidDynamicLockFlags();
		}
		return ret;
	}


}

