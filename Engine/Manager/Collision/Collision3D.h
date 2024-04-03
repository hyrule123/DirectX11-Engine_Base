#pragma once
#include "Engine/CommonType.h"
#include "Engine/define_Enum.h"

#pragma warning(disable : 26495)
#pragma warning(disable : 33010)
#pragma warning(disable : 26812)
#pragma warning(disable : 26451)
#pragma warning(disable : 6297)

//#define PX_PHYSX_STATIC_LIB
#include <PhysX/PxPhysicsAPI.h>

#pragma warning(default : 26495)
#pragma warning(default : 33010)
#pragma warning(default : 26812)
#pragma warning(default : 26451)
#pragma warning(default : 6297)

namespace ehw
{
	// 전방 선언
	class iScene;
	class GameObject;
	class Com_Collider3D;

	struct RaycastHit
	{
		GameObject* gameObject;
		bool		hasBlocking;
		float3		hitPosition;
		float3		hitNormal;
		float		hitDistance;
	};

	enum class UpdateInterval
	{
		Frame_30,
		Frame_60,
		Frame_120,
		Frame_144,
		Frame_240,

		END
	};

	constexpr std::array<float, static_cast<UINT8>(UpdateInterval::END)> g_intervals
	{
		1.f / 30.f,
		1.f / 60.f,
		1.f / 120.f,
		1.f / 144.f,
		1.f / 240.f,
	};

	class Collision3D final : public ::physx::PxSimulationEventCallback
	{
		friend class CollisionManager;

	public:
		void Initialize();
		void CollisionUpdate();

		void EnableRaycast(uint32 leftLayerIndex, uint32 rightLayerIndex, bool enable);
		void EnableCollision(uint32 leftLayerIndex, uint32 rightLayerIndex, bool enable);
		void EnableGravity(bool enable, iScene* scene, const float3& gravity) const;

		void createActorCube(GameObject* gameObject, const float3& halfExtents, physx::PxShape** outShape, bool isStatic);
		void createActorSphere(GameObject* gameObject, float radius, physx::PxShape** outShape, bool isStatic);
		void createActorCapsule(GameObject* gameObject, float radius, float height, physx::PxShape** outShape, bool isStatic);
		void changeGeometry(Com_Collider3D* collider, physx::PxShape* shape, eCollider3D_Shape type);

		bool Raycast(uint32 srcLayerIndex, const float3& origin, const float3& direction, float maxDistance, RaycastHit* outHit) const;

		void createScene(iScene* scene);
		void changeScene(iScene* scene);

	private:
		explicit Collision3D();
		virtual ~Collision3D();

		void SyncGameScene() const;
		void setupFiltering(physx::PxShape* shape, uint32 layerIndex) const;

		static physx::PxFilterFlags FilterShader(
			physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
			physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
			physx::PxPairFlags& pairFlags,
			const void* constantBlock, physx::PxU32 constantBlockSize);

		inline void setUpdateInterval(UpdateInterval interval) { _currentInterval = interval; }
		inline float getUpdateInterval() const { return g_intervals[static_cast<UINT8>(_currentInterval)]; }

		// PxSimulationEventCallback을(를) 통해 상속됨
		void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override;
		void onWake(physx::PxActor** actors, physx::PxU32 count) override;
		void onSleep(physx::PxActor** actors, physx::PxU32 count) override;
		void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override;
		void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override;
		void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) override;


	private:
		physx::PxDefaultAllocator		_allocator;
		physx::PxDefaultErrorCallback	_errorCallback;
		physx::PxFoundation* _foundation;
		physx::PxPhysics* _physics;
		physx::PxDefaultCpuDispatcher* _dispatcher;
		physx::PxScene* _currentScene;
		physx::PxMaterial* _material;
		std::array<std::bitset<32>, 32> _collisionMask;
		std::array<std::bitset<32>, 32> _raycastMask;

		
		UpdateInterval											   _currentInterval;

		std::unordered_map<iScene*, physx::PxScene*>  _scenes;
		inline static constexpr float _kDefaultDensity = 10.f;
#ifdef _DEBUG
		physx::PxPvd* _pvd{};
#endif
	};


} // namespace ehw
