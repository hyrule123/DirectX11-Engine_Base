#pragma once
#include "Engine/Game/Component/iComponent.h"

#include "Engine/Game/Component/Collider/iCollider2D.h"

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
		virtual void InternalUpdate() final {};

		virtual void OnCollisionEnter(const std::shared_ptr<iCollider2D>& _collider) {};
		virtual void OnCollisionStay(const std::shared_ptr<iCollider2D>& _collider) {};
		virtual void OnCollisionExit(const std::shared_ptr<iCollider2D>& _collider) {};

		virtual void OnTriggerEnter(const std::shared_ptr<iCollider2D>& _collider) {};
		virtual void OnTriggerStay(const std::shared_ptr<iCollider2D>& _collider) {};
		virtual void OnTriggerExit(const std::shared_ptr<iCollider2D>& _collider) {};

	private:
	};
}
