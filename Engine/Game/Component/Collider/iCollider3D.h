#pragma once
#include "Engine/Game/Component/Collider/iCollider.h"
#include "Engine/define_Enum.h"

#include "Engine/Game/Collision/PhysX.h"

namespace ehw
{
	enum class eFreezeRotationFlag : uint8
	{
		ROTATION_X = (1 << 1),
		ROTATION_Y = (1 << 2),
		ROTATION_Z = (1 << 4),
		END
	};

	class Com_Transform;
	class Collision3D;
	class iCollider3D : public iCollider
	{
	public:
		iCollider3D(eCollider3DType _col3dType);
		CLONE_DISABLE(iCollider3D);
		
		virtual ~iCollider3D();

		void OnCollisionEnter(iCollider3D* const _collider, const float3& _contactPoint);
		void OnCollisionStay(iCollider3D* const _collider, const float3& _contactPoint);
		void OnCollisionExit(iCollider3D* const _collider);

		void OnTriggerEnter(iCollider3D* const _collider);
		void OnTriggerStay(iCollider3D* const _collider);
		void OnTriggerExit(iCollider3D* const _collider);

		eResult Serialize_Json(JsonSerializer* _ser) const override { return eResult(); };
		eResult DeSerialize_Json(const JsonSerializer* _ser) override { return eResult(); };

		void Init() override;
		void Awake() override;
		void OnDestroy() override;
		

		MATRIX GetColliderMatrix() override { return MATRIX{}; };

		

		//씬 변경시 dontdestroy에 의해 제거되지 않고 씬만 변경 되었을 경우
		void			SceneChanged();
		void			EnableGravity(bool enable);
		bool			isGravityEnabled() const { return m_isGravityEnable; }

		
		



		//Start 호출 되기 전에만 동작함.
		void SetSyncScaleToTransfom(bool enable) { m_isSyncScaleToTransform = enable; }
		inline Com_Transform* GetTransform() { return m_myTransform; }

	protected:
		inline bool IsSyncScaleToTransform() const { return m_isSyncScaleToTransform; }
		inline Collision3D* GetCollision3D() { return m_collision3D; }
		inline physx::PxActor* GetPxActor() { return m_pxActor; }
		inline physx::PxActor* GetPxActor() const { return m_pxActor; }

	private:
		//각자 상속해서 자신에게 맞는 PxActor를 생성 후 반환한다.
		//호출하는 함수: Init()
		virtual physx::PxActor* CreateActor() = 0;

	private:
		eCollider3DType				m_colliderType;
		Com_Transform*				m_myTransform;
		Collision3D*				m_collision3D;
		bool						m_isSyncScaleToTransform;
		
		//생성시점: Init()
		physx::PxActor*		m_pxActor;

		uint		 m_collisionCount;
		

		float		 m_mass;
		float		 m_maxVelocity;

		//renderer::WireFrameCB						_wireFrameData;
		bool			m_isGravityEnable;
		
		bool			m_isKinematic;
	};
}


