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

	public:
		Script(const std::string_view key);
		Script(const Script& _other) = default;
		virtual ~Script();

		//Script의 경우 Serialize 하지 않는 경우도 있으므로 일단 재정의해둔다.
		virtual eResult serialize_json(JsonSerializer* _ser) const override { return eResult::Success; }
		virtual eResult deserialize_json(const JsonSerializer* _ser) override { return eResult::Success; }

		virtual void init() {};
		virtual void Awake() override {};
		virtual void update() override {};
		virtual void final_update() final {};

		virtual void OnCollisionEnter(const s_ptr<Collider>& _collider, const float3& _hitPoint) {};
		virtual void OnCollisionStay(const s_ptr<Collider>& _collider, const float3& _hitPoint) {};
		virtual void OnCollisionExit(const s_ptr<Collider>& _collider) {};

		virtual void OnTriggerEnter(const s_ptr<Collider>& _collider) {};
		virtual void OnTriggerStay(const s_ptr<Collider>& _collider) {};
		virtual void OnTriggerExit(const s_ptr<Collider>& _collider) {};
	};
}
