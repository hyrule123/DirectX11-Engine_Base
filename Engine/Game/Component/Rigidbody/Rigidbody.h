#pragma once
#include <Engine/Game/Component/Component.h>
#include <Engine/Game/Collision/PxActorWrapper.h>

//이 클래스가 담당하는일
//Rigidbody 생성, 삭제
//Rigidbody를 Collision3D에 등록(on_enable), 해제(OnDisable)

namespace core
{
	class Rigidbody :
		public Component
	{
		CLASS_INFO(Rigidbody, Component);
		BASE_COMPONENT(eComponentOrder::Rigidbody);

	public:
		Rigidbody();
		virtual ~Rigidbody();

		virtual eResult serialize_json(JsonSerializer* _ser) const final { return eResult{}; };
		virtual eResult deserialize_json(const JsonSerializer* _ser) final { return eResult{}; };

		void init() final;
		void final_update() final{}

		void on_enable() final;
		void on_disable() final;
		void on_destroy() final;

		bool attach_collider_shape(physx::PxShape* _pxShape);
		void detach_collider_shape(physx::PxShape* _pxShape);

		//Collision3D에서 호출
		void sync_to_physX_global_pose();
		void fetch_from_physX_global_pose(const physx::PxTransform& _pxTransform);

		void			set_gravity(bool enable);
		bool			is_gravity_enabled() const;

	protected:
		virtual physx::PxRigidActor* create_rigidbody() = 0;
		physx::PxRigidActor* get_px_actor() { return m_rigid_actor.Get(); }
		const physx::PxRigidActor* get_px_actor() const { return m_rigid_actor.Get(); }

		//Shape가 추가되거나 제거될 경우 질량(혹은 밀도)를 재계산해줘야 함
		inline bool is_shapes_modified() const { return m_shapes_modified; }
		inline void set_shapes_modified(bool _modified) { m_shapes_modified = _modified; }

	private:
		PxActorWrapper<physx::PxRigidActor> m_rigid_actor;

		bool m_shapes_modified;
	};
}

