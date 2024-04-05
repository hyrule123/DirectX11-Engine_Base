#pragma once
#include "Engine/CommonType.h"
#include "Engine/define_Enum.h"

#include "Engine/Game/Collision/PhysX.h"

//작동방식
//OnEnable 시점에 PhysX에 등록
//OnDisable 시점에 PhysX에서 해제


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

	class Collision3D final : public ::physx::PxSimulationEventCallback
	{
		friend class CollisionSystem;
	private:
		Collision3D(CollisionSystem* const _owner);
		virtual ~Collision3D();

	public:
		static constexpr float s_defaultDensity = 10.f;
		static constexpr std::array<float, (int)UpdateInterval::END> s_physxUpdateIntervals
		{
			1.f / 30.f,
			1.f / 60.f,
			1.f / 120.f,
			1.f / 144.f,
			1.f / 240.f,
		};
	
		void Init();
		void CollisionUpdate();

		void EnableGravity(bool enable, const float3& gravity) const;

		void createActorCube(GameObject* gameObject, const float3& halfExtents, physx::PxShape** outShape, bool isStatic);
		void createActorSphere(GameObject* gameObject, float radius, physx::PxShape** outShape, bool isStatic);
		void createActorCapsule(GameObject* gameObject, float radius, float height, physx::PxShape** outShape, bool isStatic);
		void changeGeometry(Com_Collider3D* collider, physx::PxShape* shape, eCollider3D_Shape type);

		bool Raycast(uint32 srcLayerIndex, const float3& origin, const float3& direction, float maxDistance, RaycastHit* outHit) const;

		void createScene(iScene* scene);
		void changeScene(iScene* scene);

	private:
		void SyncGameScene() const;
		void setupFiltering(physx::PxShape* shape, uint32 layerIndex) const;

		static physx::PxFilterFlags FilterShader(
			physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
			physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
			physx::PxPairFlags& pairFlags,
			const void* constantBlock, physx::PxU32 constantBlockSize);

		inline void setUpdateInterval(UpdateInterval interval) { m_curUpdateInterval = interval; }
		inline float getUpdateInterval() const { return s_physxUpdateIntervals[static_cast<uint8>(m_curUpdateInterval)]; }

		// PxSimulationEventCallback을(를) 통해 상속됨
		void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override;
		void onWake(physx::PxActor** actors, physx::PxU32 count) override;
		void onSleep(physx::PxActor** actors, physx::PxU32 count) override;
		void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override;
		void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override;
		void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) override;

	private:
		template <typename T>
		static inline void PxRelease(T* _pxPtr) { if (_pxPtr) { _pxPtr->Release(); } }

	private:
		CollisionSystem* const m_owner;

		std::unique_ptr<physx::PxScene> m_currentScene;

		//정적 마찰력, 동적 마찰력, 반발력 프리셋.
		//충돌체(PxShape 생성 시 필요), 프리셋 값을 변경 시 해당 material을 사용한 모든 충돌체의 프리셋 설정이 변경됨.
		std::shared_ptr<physx::PxMaterial> m_pxMaterial;
		
		UpdateInterval	m_curUpdateInterval;
	};


} // namespace ehw
