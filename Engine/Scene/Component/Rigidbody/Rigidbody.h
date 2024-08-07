#pragma once
#include "Engine/Scene/Component/Component.h"
#include "Engine/Scene/Collision/PxActorWrapper.h"

//이 클래스가 담당하는일
//Rigidbody 생성, 삭제
//Rigidbody를 Collision3D에 등록(OnEnable), 해제(OnDisable)

namespace ehw
{
	class Rigidbody :
		public Component<Rigidbody, eComponentCategory::Rigidbody>
	{
	public:
		Rigidbody();
		virtual ~Rigidbody();

		virtual eResult Serialize_Json(JsonSerializer* _ser) const final { return eResult{}; };
		virtual eResult DeSerialize_Json(const JsonSerializer* _ser) final { return eResult{}; };

		void Init() final;
		void FinalUpdate() final{}

		void OnEnable() final;
		void OnDisable() final;
		void OnDestroy() final;

		bool AttachColliderShape(physx::PxShape* _pxShape);
		void DetachColliderShape(physx::PxShape* _pxShape);

		//Collision3D에서 호출
		void SyncToPhysXGlobalPose();
		void FetchFromPhysXGlobalPose(const physx::PxTransform& _pxTransform);

		void			EnableGravity(bool enable);
		bool			IsGravityEnabled() const;

	protected:
		virtual physx::PxRigidActor* CreateRigidbody() = 0;
		physx::PxRigidActor* GetPxActor() { return m_rigidActor.Get(); }
		const physx::PxRigidActor* GetPxActor() const { return m_rigidActor.Get(); }

		//Shape가 추가되거나 제거될 경우 질량(혹은 밀도)를 재계산해줘야 함
		inline bool IsShapesModified() const { return m_ShapesModified; }
		inline void SetShapesModified(bool _modified) { m_ShapesModified = _modified; }

	private:
		PxActorWrapper<physx::PxRigidActor> m_rigidActor;

		bool m_ShapesModified;
	};
}

