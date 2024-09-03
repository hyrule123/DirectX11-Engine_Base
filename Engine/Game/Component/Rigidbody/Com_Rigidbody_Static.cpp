#include "Engine/Game/Component/Rigidbody/Com_Rigidbody_Static.h"

#include "Engine/Game/Collision/PhysXInstance.h"

#include "Engine/Game/GameObject.h"
#include "Engine/Game/Component/Com_Transform.h"

namespace ehw
{
	Com_Rigidbody_Static::Com_Rigidbody_Static()
		: Rigidbody(ClassInfo<Com_Rigidbody_Static>::name())
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
