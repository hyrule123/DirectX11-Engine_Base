#pragma once
#include <Engine/Game/Component/Rigidbody/Rigidbody.h>

namespace core
{
	class Com_Rigidbody_Static :
		public Rigidbody
	{
		CLASS_INFO(Com_Rigidbody_Static, Rigidbody);
		REGISTER_FACTORY(Com_Rigidbody_Static);
		
	public:
		Com_Rigidbody_Static();
		virtual ~Com_Rigidbody_Static();

	protected:
		physx::PxRigidActor* CreateRigidbody() final;
	};
}

