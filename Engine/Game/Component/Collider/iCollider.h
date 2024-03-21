#pragma once
#include "Engine/Game/Component/iComponent.h"

#include "Engine/CommonStruct.h"

#include <unordered_set>

namespace ehw
{
	class iCollider
		: public Component<iCollider, eComponentCategory::Collider>
	{
		friend class Collision2D;
		friend class Collision3D;
	public:
		iCollider(eDimensionType _dimension);
		iCollider(const iCollider& _collider) = default;
		virtual ~iCollider();

		virtual void Awake() override;

		inline eDimensionType GetColliderType() const { return m_dimension; }
		inline bool IsTriggerMode() const { return m_isTriggerMode; }

		virtual void OnCollisionEnter(const std::shared_ptr<iCollider>& _collider, const Vector2 _contactPoint) {}
		virtual void OnCollisionStay(const std::shared_ptr<iCollider>& _collider, const Vector2 _contactPoint) {}
		virtual void OnCollisionExit(const std::shared_ptr<iCollider>& _collider) {}

		virtual void OnTriggerEnter(const std::shared_ptr<iCollider>& _collider) {}
		virtual void OnTriggerStay(const std::shared_ptr<iCollider>& _collider) {}
		virtual void OnTriggerExit(const std::shared_ptr<iCollider>& _collider) {}

		

	protected:
		inline Com_Transform* GetMyTransform() { return m_transform.get(); }
		
	private:
		virtual void DebugRender() = 0;

		eDimensionType m_dimension;
		bool m_isTriggerMode;

		//Transform을 많이 사용하므로 아예 주소를 받아 놓는다.
		std::shared_ptr<Com_Transform> m_transform;
	};
}


