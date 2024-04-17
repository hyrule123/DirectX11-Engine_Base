#include "Engine/Game/Component/Rigidbody/Com_Rigidbody_Static.h"

#include "Engine/Game/Collision/PhysXInstance.h"

#include "Engine/Game/GameObject.h"
#include "Engine/Game/Component/Transform/Com_Transform.h"

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
		Com_Transform* tr = GetOwner()->Transform();

		physx::PxTransform pxTr{};
		pxTr.p = tr->GetWorldPosition();
		pxTr.q = tr->GetLocalRotation();

		return PhysXInstance::GetInst().createRigidStatic(pxTr);
	}
}
