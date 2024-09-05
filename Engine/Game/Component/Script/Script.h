#pragma once
#include "Engine/Game/Component/Component.h"

#include "Engine/Game/Component/Collider/Collider2D.h"

namespace ehw
{
	class Script 
		: public Component<Script, eComponentCategory::Scripts>
	{
		CLASS_NAME(Script);
	public:
		Script(const std::string_view key);
		Script(const Script& _other) = default;
		virtual ~Script();

		//Script의 경우 Serialize 하지 않는 경우도 있으므로 일단 재정의해둔다.
		virtual eResult Serialize_Json(JsonSerializer* _ser) const override { return eResult::Success; }
		virtual eResult DeSerialize_Json(const JsonSerializer* _ser) override { return eResult::Success; }

		virtual void Init() {};
		virtual void Awake() override {};
		virtual void Update() override {};
		virtual void FinalUpdate() final {};

		virtual void OnCollisionEnter(Collider* const _collider, const float3& _hitPoint) {};
		virtual void OnCollisionStay(Collider* const _collider, const float3& _hitPoint) {};
		virtual void OnCollisionExit(Collider* const _collider) {};

		virtual void OnTriggerEnter(Collider* const _collider) {};
		virtual void OnTriggerStay(Collider* const _collider) {};
		virtual void OnTriggerExit(Collider* const _collider) {};
	};
}
