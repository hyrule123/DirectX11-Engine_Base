#include "Engine/Game/Collision/PhysXInstance.h"

#include "Engine/Util/AtExit.h"

#include "Engine/define_Macro.h"

#include "Engine/Game/Component/Collider/Collider3D.h"

namespace core
{
	PhysXInstance::PhysXInstance()
		: m_allocator{}
		, m_errorCallback{}
		, m_foundation{ nullptr }
		, m_physics{ nullptr }
		, m_dispatcher{ nullptr }
		, m_PVD{ nullptr }	//PhysX Visualization Debugger
	{
		using namespace physx;
		//CreateBuffer PxFoundation
		m_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_allocator, m_errorCallback);
		ASSERT(m_foundation, "PxFoundation 생성 실패");

		bool enable = false;

		//CreateBuffer PVD
#ifdef _DEBUG
		enable = true;

		m_PVD = PxCreatePvd(*m_foundation);
		ASSERT(m_PVD, "pvd 생성 실패");

		PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
		m_PVD->connect(*transport, PxPvdInstrumentationFlag::eALL);
#endif

		//CreateBuffer PhysX instance
		m_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_foundation, PxTolerancesScale{}, enable, m_PVD);

		ASSERT(m_physics, "PhysX 인스턴스 생성 실패");


#ifdef _DEBUG
		if (false == PxInitExtensions(*m_physics, m_PVD))
		{
			ASSERT(false, "PxInitExtensions 함수 에러 발생");
		}
#endif

		//CreateBuffer dispatcher 
		constexpr uint32 kThreadCount = 2;
		m_dispatcher = PxDefaultCpuDispatcherCreate(kThreadCount);
		ASSERT(m_dispatcher, "physx dispatcher 생성 실패");
	}

	PhysXInstance::~PhysXInstance()
	{
		using namespace physx;
		PX_RELEASE(m_dispatcher);
		PX_RELEASE(m_physics);
#ifdef _DEBUG
		PxCloseExtensions();
		if (m_PVD)
		{
			PxPvdTransport* transport = m_PVD->getTransport();
			PX_RELEASE(m_PVD);
			PX_RELEASE(transport);
		}
#endif
		PX_RELEASE(m_foundation);
	}
}
