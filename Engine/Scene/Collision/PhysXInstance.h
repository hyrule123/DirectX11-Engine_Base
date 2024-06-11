#pragma once
#include "Engine/Util/StaticSingleton.h"
#include "Engine/Scene/Collision/PhysX.h"

namespace ehw
{
	class iCollider3D;
	class PhysXInstance : public StaticSingleton<PhysXInstance>
	{
		friend class StaticSingleton<PhysXInstance>;
		friend class GameEngine;
	private:
		PhysXInstance();
		~PhysXInstance();
		void Init();
		void Release();

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

