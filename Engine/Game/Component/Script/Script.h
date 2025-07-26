#pragma once
#include <Engine/Game/Component/Component.h>

#include <Engine/Game/Component/Collider/Collider2D.h>

namespace core
{
	class Script 
		: public Component
	{
		CLASS_INFO(Script, Component);
		BASE_COMPONENT(eComponentOrder::Script);

		NO_SERIALIZE_BINARY;
		NO_SERIALIZE_JSON;

	public:
		Script();
		Script(const Script& _other) = default;
		virtual ~Script();

		virtual void init() {};
		virtual void awake() override {};
		virtual void update() override {};
		virtual void final_update() final {};

		virtual void on_collision_enter(const s_ptr<Collider>& _collider, const float3& _hitPoint) {};
		virtual void on_collision_stay(const s_ptr<Collider>& _collider, const float3& _hitPoint) {};
		virtual void on_collision_exit(const s_ptr<Collider>& _collider) {};

		virtual void on_trigger_enter(const s_ptr<Collider>& _collider) {};
		virtual void on_trigger_stay(const s_ptr<Collider>& _collider) {};
		virtual void on_trigger_exit(const s_ptr<Collider>& _collider) {};
	};
}
