#pragma once
#include "Engine/Scene/Component/Rigidbody/Rigidbody.h"

namespace ehw
{
	class Com_Rigidbody_Static :
		public Rigidbody
	{
	public:
		Com_Rigidbody_Static();
		virtual ~Com_Rigidbody_Static();

	protected:
		physx::PxRigidActor* CreateRigidbody() final;
	};
}

