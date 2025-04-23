#pragma once
#include "Engine/Util/Singleton.h"
#include "Engine/Game/Collision/PhysX.h"

namespace core
{
	class Collider3D;
	class PhysXInstance : public Singleton<PhysXInstance>
	{
		friend class Singleton<PhysXInstance>;
		friend class GameEngine;
	private:
		PhysXInstance();
		~PhysXInstance();

	public:
		inline physx::PxPhysics& GetPhysX() { return *m_physics; };
		inline physx::PxDefaultCpuDispatcher* GetCPUDispatcher() { return m_dispatcher; }

	private:
		physx::PxDefaultAllocator		m_allocator;
		physx::PxDefaultErrorCallback	m_errorCallback;

		physx::PxFoundation*				m_foundation;
		physx::PxPhysics*				m_physics;

		physx::PxDefaultCpuDispatcher*	m_dispatcher;

		//Physx Visualization Debugger
		physx::PxPvd* m_PVD;
	};
}

