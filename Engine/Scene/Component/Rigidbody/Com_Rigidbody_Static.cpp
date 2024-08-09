#include "Engine/Scene/Component/Rigidbody/Com_Rigidbody_Static.h"

#include "Engine/Scene/Collision/PhysXInstance.h"

#include "Engine/Scene/GameObject.h"
#include "Engine/Scene/Component/Transform/Com_Transform.h"

namespace ehw
{
	Com_Rigidbody_Static::Com_Rigidbody_Static()
	{
	}

	Com_Rigidbody_Static::~Com_Rigidbody_Static()
	{
	}

	physx::PxRigidActor* Com_Rigidbody_Static::CreateRigidbody()
	{
		Com_Transform* tr = gameObject()->Transform();

		physx::PxTransform pxTr{};
		pxTr.p = tr->GetWorldPosition();
		pxTr.q = tr->GetLocalRotation();

		return PhysXInstance::GetInst().GetPhysX().createRigidStatic(pxTr);
	}
}
