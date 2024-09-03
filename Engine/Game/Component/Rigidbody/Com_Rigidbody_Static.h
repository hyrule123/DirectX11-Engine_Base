#pragma once
#include "Engine/Game/Component/Rigidbody/Rigidbody.h"

namespace ehw
{
	class Com_Rigidbody_Static :
		public Rigidbody
	{
		REGISTER_CLASS_INFO(Com_Rigidbody_Static);
		SET_INSTANCE_ABLE(Com_Rigidbody_Static);
	public:
		Com_Rigidbody_Static();
		virtual ~Com_Rigidbody_Static();

	protected:
		physx::PxRigidActor* CreateRigidbody() final;
	};
}

