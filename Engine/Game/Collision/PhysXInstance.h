#pragma once
#include "Engine/Game/Collision/PhysX.h"

namespace ehw
{
	class iCollider3D;
	class PhysXInstance
	{
		friend class SceneManager;

	public:
		inline static physx::PxPhysics& GetInst() { return *m_physics; };
		inline static physx::PxDefaultCpuDispatcher* GetCPUDispatcher() { return m_dispatcher; }


		//SceneManager에서 호출.
		static void Init();
		static void Release();



	private:
		static physx::PxDefaultAllocator		m_allocator;
		static physx::PxDefaultErrorCallback	m_errorCallback;

		static physx::PxFoundation*				m_foundation;
		static physx::PxPhysics*				m_physics;

		static physx::PxDefaultCpuDispatcher*	m_dispatcher;

		

#ifdef _DEBUG
		//Physx Visualization Debugger
		static physx::PxPvd* m_PVD;
#endif

	private:
		PhysXInstance() = delete;
		~PhysXInstance() = delete;
	};
}

