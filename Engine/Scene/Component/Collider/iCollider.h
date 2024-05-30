#pragma once
#include "Engine/Scene/Component/iComponent.h"

#include "Engine/Common.h"

#include <unordered_set>

namespace ehw
{
	class Com_Transform;
	class CollisionSystem;
	class iCollider
		: public Component<iCollider, eComponentCategory::Collider>
	{
		friend class Collision2D;
		friend class Collision3D;
		friend class ContactPair;
	public:
		iCollider(eDimensionType _dimension);
		iCollider(const iCollider& _collider) = default;
		virtual ~iCollider();

		virtual void Init() override;
		virtual void Awake() override;

		void OnCollisionEnter(iCollider* _collider, const float3& _contactPoint);
		void OnCollisionStay(iCollider* _collider, const float3& _contactPoint);
		void OnCollisionExit(iCollider* _collider);
		void OnTriggerEnter(iCollider* _collider);
		void OnTriggerStay(iCollider* _collider);
		void OnTriggerExit(iCollider* _collider);

		inline eDimensionType GetColliderType() const { return m_dimension; }
		inline bool IsTriggerMode() const { return m_isTriggerMode; }

		inline bool IsColliding() const { return (0 < m_collisionCount); }
		
		virtual MATRIX GetColliderMatrix() = 0;

	protected:
		inline Com_Transform* GetTransform() { return m_transform; }
		inline CollisionSystem* GetCollisionSystem() { return m_collisionSystem; }
		inline void AddCollisionCount() { ++m_collisionCount; }
		inline void SubCollisionCount();
		
	private:
		eDimensionType m_dimension;
		bool m_isTriggerMode;

		//Transform을 많이 사용하므로 아예 주소를 받아 놓는다.
		Com_Transform* m_transform;
		CollisionSystem* m_collisionSystem;

		int m_collisionCount;
	};

	inline void iCollider::SubCollisionCount()
	{
		--m_collisionCount;
		if (m_collisionCount < 0)
		{
			m_collisionCount = 0;
			ASSERT_DEBUG(false, "Collision Count가 음수로 내려갔습니다.");
		}
	}
}


