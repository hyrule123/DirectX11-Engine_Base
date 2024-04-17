#pragma once
#include "Engine/Game/Component/iComponent.h"
#include "Engine/Game/Collision/PxActorWrapper.h"

//이 클래스가 담당하는일
//Rigidbody 생성, 삭제
//Rigidbody를 Collision3D에 등록(OnEnable), 해제(OnDisable)

namespace ehw
{
	class iRigidbody :
		public Component<iRigidbody, eComponentCategory::Rigidbody>
	{
	public:
		iRigidbody();
		virtual ~iRigidbody();

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
		void UpdateGlobalPose();

		void			EnableGravity(bool enable);
		bool			IsGravityEnabled() const;

	protected:
		virtual physx::PxRigidActor* CreateRigidbody() = 0;
		physx::PxRigidActor* GetPxActor() { return m_rigidActor.Get(); }
		const physx::PxRigidActor* GetPxActor() const { return m_rigidActor.Get(); }

	private:
		PxActorWrapper<physx::PxRigidActor> m_rigidActor;
	};
}

