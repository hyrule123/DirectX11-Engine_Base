#include "Engine/Scene/Collision/PhysXInstance.h"

#include "Engine/Util/AtExit.h"

#include "Engine/define_Macro.h"

#include "Engine/Scene/Component/Collider/iCollider3D.h"

namespace ehw
{
	PhysXInstance::PhysXInstance()
		: m_allocator{}
		, m_errorCallback{}
		, m_foundation{ nullptr }
		, m_physics{ nullptr }
		, m_dispatcher{ nullptr }
		, m_PVD{ nullptr }	//PhysX Visualization Debugger
	{}

	PhysXInstance::~PhysXInstance()
	{
	}

	void PhysXInstance::Init()
	{
		using namespace physx;
		AtExit::AddFunc(std::bind(&PhysXInstance::Release, this));


		//Create PxFoundation
		m_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_allocator, m_errorCallback);
		ASSERT(m_foundation, "PxFoundation 생성 실패");

		bool enable = false;

		//Create PVD
#ifdef _DEBUG
		enable = true;

		m_PVD = PxCreatePvd(*m_foundation);
		ASSERT(m_PVD, "pvd 생성 실패");

		PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
		m_PVD->connect(*transport, PxPvdInstrumentationFlag::eALL);
#endif

		//Create PhysX instance
		m_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_foundation, PxTolerancesScale{}, enable, m_PVD);

		ASSERT(m_physics, "PhysX 인스턴스 생성 실패");


#ifdef _DEBUG
		if (false == PxInitExtensions(*m_physics, m_PVD))
		{
			ASSERT(false, "PxInitExtensions 함수 에러 발생");
		}
#endif

		//Create dispatcher 
		constexpr uint32 kThreadCount = 2;
		m_dispatcher = PxDefaultCpuDispatcherCreate(kThreadCount);
		ASSERT(m_dispatcher, "physx dispatcher 생성 실패");
	}


	void PhysXInstance::Release()
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
