#pragma once
#include "../iComponent.h"



#include "../Collider/iCollider2D.h"

namespace ehw
{
	class iScript : public iComponent
	{
	public:
		iScript();
		iScript(const iScript& _other) = default;
		virtual ~iScript();

		virtual void Init() {};
		virtual void Awake() override {};
		virtual void Update() override {};
		virtual void InternalUpdate() final {};

		virtual void OnCollisionEnter(iCollider2D* _collider) {};
		virtual void OnCollisionStay(iCollider2D* _collider) {};
		virtual void OnCollisionExit(iCollider2D* _collider) {};

		virtual void OnTriggerEnter(iCollider2D* _collider) {};
		virtual void OnTriggerStay(iCollider2D* _collider) {};
		virtual void OnTriggerExit(iCollider2D* _collider) {};

	private:
	};
}
