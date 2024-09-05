#pragma once
#include "Engine/Game/Component/Rigidbody/Rigidbody.h"

namespace ehw
{
	class Com_Rigidbody_Static :
		public Rigidbody
	{
		CLASS_NAME(Com_Rigidbody_Static);
		REGISTER_INSTANCE_DEFAULT(Com_Rigidbody_Static);
		
	public:
		Com_Rigidbody_Static();
		virtual ~Com_Rigidbody_Static();

	protected:
		physx::PxRigidActor* CreateRigidbody() final;
	};
}

