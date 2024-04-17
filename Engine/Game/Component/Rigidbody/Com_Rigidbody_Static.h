#pragma once
#include "Engine/Game/Component/Rigidbody/iRigidbody.h"

namespace ehw
{
	class Com_Rigidbody_Static :
		public iRigidbody
	{
	public:
		Com_Rigidbody_Static();
		virtual ~Com_Rigidbody_Static();

	protected:
		physx::PxRigidActor* CreateRigidbody() final;
	};
}

