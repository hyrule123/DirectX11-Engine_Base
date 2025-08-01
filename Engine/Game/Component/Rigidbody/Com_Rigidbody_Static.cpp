
#include "Com_Rigidbody_Static.h"

#include <Engine/Game/Collision/PhysXInstance.h>

#include <Engine/Game/GameObject.h>
#include <Engine/Game/Component/Transform.h>

namespace core
{
	Com_Rigidbody_Static::Com_Rigidbody_Static()
		: Rigidbody()
	{
	}

	Com_Rigidbody_Static::~Com_Rigidbody_Static()
	{
	}

	physx::PxRigidActor* Com_Rigidbody_Static::create_rigidbody()
	{
		if (get_owner())
		{
			s_ptr<Transform> tr = get_owner()->get_component<Transform>();
			physx::PxTransform pxTr{};
			pxTr.p = tr->get_world_position();
			pxTr.q = tr->get_local_rotation();
			return PhysXInstance::get_inst().GetPhysX().createRigidStatic(pxTr);
		}

		return nullptr;
	}
}
