#pragma once
#include "Engine/Game/Component/BaseComponent.h"
#include "Engine/Game/Collision/PxActorWrapper.h"

//이 클래스가 담당하는일
//Rigidbody 생성, 삭제
//Rigidbody를 Collision3D에 등록(OnEnable), 해제(OnDisable)

namespace core
{
#define RIGIDBODY_COMPONENT BaseComponent<Rigidbody, eComponentCategory::Rigidbody>

	class Rigidbody :
		public RIGIDBODY_COMPONENT
	{
		CLASS_INFO(Rigidbody, RIGIDBODY_COMPONENT);
	public:
		Rigidbody(const std::string_view key);
		virtual ~Rigidbody();

		virtual eResult serialize_json(JsonSerializer* _ser) const final { return eResult{}; };
		virtual eResult deserialize_json(const JsonSerializer* _ser) final { return eResult{}; };

		void init() final;
		void final_update() final{}

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

