#include "Engine/Game/Component/Rigidbody/Com_Rigidbody_Static.h"

#include "Engine/Game/Collision/PhysXInstance.h"

#include "Engine/Game/GameObject.h"
#include "Engine/Game/Component/Transform.h"

namespace ehw
{
	Com_Rigidbody_Static::Com_Rigidbody_Static()
		: Rigidbody(Com_Rigidbody_Static::concrete_class_name)
	{
	}

	Com_Rigidbody_Static::~Com_Rigidbody_Static()
	{
	}

	physx::PxRigidActor* Com_Rigidbody_Static::CreateRigidbody()
	{
		Transform* tr = gameObject()->transform();

		physx::PxTransform pxTr{};
		pxTr.p = tr->get_world_position();
		pxTr.q = tr->get_local_rotation();

		return PhysXInstance::get_inst().GetPhysX().createRigidStatic(pxTr);
	}
}
