#pragma once
#include "Engine/CommonType.h"
#include "Engine/define_Enum.h"

#include "Engine/DefaultShader/Debug/Debug.hlsli"

#include "Engine/Game/Collision/PhysX.h"

//작동방식
//OnEnable 시점에 PhysX에 등록
//OnDisable 시점에 PhysX에서 해제


namespace ehw
{
	// 전방 선언
	class iScene;
	class GameObject;
	class Com_Transform;
	class iCollider3D;
	class CollisionSystem;

	struct RaycastHit
	{
		iCollider3D* hitCollider;
		bool		hasBlocking;
		float3		hitPosition;
		float3		hitNormal;
		float		hitDistance;
	};

	enum class eFilterType
	{
		Collision,
		Raycast
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
	public:
		Collision3D(CollisionSystem* const _owner);
		virtual ~Collision3D();

		void Init();
		void CreatePxScene();
		void FixedUpdate();
		void GameSceneToPxScene();
		void PxSceneToGameScene();
		void Render();
		void FrameEnd();

		static constexpr float m_defaultDensity = 10.f;
		static constexpr std::array<float, (int)UpdateInterval::END> m_physxUpdateIntervals
		{
			1.f / 30.f,
			1.f / 60.f,
			1.f / 120.f,
			1.f / 144.f,
			1.f / 240.f,
		};

		inline physx::PxMaterial* GetDefaultPxMaterial() { return m_defaultPxMaterial; }

		bool AddPxActor(physx::PxActor* _actor);
		void RemovePxActor(physx::PxActor* _actor);

		void EnableGravity(bool _enable, const float3& _gravity) const;

		//void changeGeometry(iCollider3D* _collider, physx::PxShape* shape, eCollider3D_Shape type);

		bool Raycast(uint32 _srcLayerIndex, const float3& _origin, const float3& _direction, float _maxDistance, RaycastHit* _outHit) const;
		
		physx::PxFilterData GetCollisionFilterData(uint32 _layer);
		physx::PxFilterData GetRaycastFilterData(uint32 _layer);

	private:
		void FetchTransformSyncDataFromPxActor();

		static physx::PxFilterFlags FilterShader(
			physx::PxFilterObjectAttributes _attributes0, physx::PxFilterData _filterData0,
			physx::PxFilterObjectAttributes _attributes1, physx::PxFilterData _filterData1,
			physx::PxPairFlags& _pairFlags,
			const void* _constantBlock, physx::PxU32 _constantBlockSize);

		inline void setUpdateInterval(UpdateInterval _interval) { m_curUpdateInterval = _interval; }
		inline float getUpdateInterval() const { return m_physxUpdateIntervals[static_cast<uint8>(m_curUpdateInterval)]; }

		// PxSimulationEventCallback을(를) 통해 상속됨
		void onConstraintBreak(physx::PxConstraintInfo* _constraints, physx::PxU32 _count) override;
		void onWake(physx::PxActor** _actors, physx::PxU32 _count) override;
		void onSleep(physx::PxActor** _actors, physx::PxU32 _count) override;
		void onContact(const physx::PxContactPairHeader& _pairHeader, const physx::PxContactPair* _pairs, physx::PxU32 _nbPairs) override;
		void onTrigger(physx::PxTriggerPair* _pairs, physx::PxU32 _count) override;
		void onAdvance(const physx::PxRigidBody* const* _bodyBuffer, const physx::PxTransform* _poseBuffer, const physx::PxU32 _count) override;

	private:
		template <typename T>
		static inline void PxRelease(T* _pxPtr) { if (_pxPtr) { _pxPtr->Release(); } }

	private:
		struct tTransformSyncData
		{
			Com_Transform* transform;
			Quaternion LocalRotation;
			float3 WorldPosition;
		};

		CollisionSystem* const m_collisionSystem;
		physx::PxScene* m_pxScene;

		//표면 설정 프리셋
		//->정적 마찰력, 동적 마찰력, 반발력 프리셋.
		//충돌체(PxShape 생성 시 필요), 프리셋 값을 변경 시 해당 material을 사용한 모든 충돌체의 프리셋 설정이 변경됨.
		physx::PxMaterial* m_defaultPxMaterial;
		UpdateInterval	m_curUpdateInterval;
		float m_accumulatedDeltaTime;

		bool m_isFixedUpdated;
		std::vector<tTransformSyncData> m_transformSyncData;

		std::array<std::vector<tDebugDrawData>, (int)eCollider3D_Shape::END> m_debugInstancingData;
	};


} // namespace ehw
