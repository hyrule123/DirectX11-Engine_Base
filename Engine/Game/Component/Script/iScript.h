#pragma once
#include "../iComponent.h"



#include "../Collider/iCollider2D.h"

namespace ehw
{
	class iScript 
		: public Component<iScript, eComponentCategory::Scripts>
	{
	public:
		iScript();
		iScript(const iScript& _other) = default;
		virtual ~iScript();

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
