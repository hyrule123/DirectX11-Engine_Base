#pragma once
#include "Engine/Scene/Component/iComponent.h"

#include "Engine/Scene/Component/Collider/iCollider2D.h"

namespace ehw
{
	class iScript 
		: public Component<iScript, eComponentCategory::Scripts>
	{
	public:
		iScript();
		iScript(const iScript& _other) = default;
		virtual ~iScript();

		//Script의 경우 Serialize 하지 않는 경우도 있으므로 일단 재정의해둔다.
		virtual eResult Serialize_Json(JsonSerializer* _ser) const override { return eResult::Success; }
		virtual eResult DeSerialize_Json(const JsonSerializer* _ser) override { return eResult::Success; }

		virtual void Init() {};
		virtual void Awake() override {};
		virtual void Update() override {};
		virtual void FinalUpdate() final {};

		virtual void OnCollisionEnter(iCollider* const _collider, const float3& _hitPoint) {};
		virtual void OnCollisionStay(iCollider* const _collider, const float3& _hitPoint) {};
		virtual void OnCollisionExit(iCollider* const _collider) {};

		virtual void OnTriggerEnter(iCollider* const _collider) {};
		virtual void OnTriggerStay(iCollider* const _collider) {};
		virtual void OnTriggerExit(iCollider* const _collider) {};
	};
}
