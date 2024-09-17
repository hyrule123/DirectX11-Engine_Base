#pragma once
#include "Engine/Game/Component/Component.h"

#include "Engine/Common.h"

#include <unordered_set>

namespace ehw
{
	class Transform;
	class CollisionSystem;
	class Collider
		: public Component<Collider, eComponentCategory::Collider>
	{
		friend class Collision2D;
		friend class Collision3D;
		friend class ContactPair;
	public:
		Collider(const std::string_view key, eDimensionType _dimension);
		Collider(const Collider& _collider) = default;
		virtual ~Collider();

		virtual void init() override;
		virtual void Awake() override;

		void OnCollisionEnter(Collider* _collider, const float3& _contactPoint);
		void OnCollisionStay(Collider* _collider, const float3& _contactPoint);
		void OnCollisionExit(Collider* _collider);
		void OnTriggerEnter(Collider* _collider);
		void OnTriggerStay(Collider* _collider);
		void OnTriggerExit(Collider* _collider);

		inline eDimensionType GetColliderType() const { return m_dimension; }
		inline bool IsTriggerMode() const { return m_isTriggerMode; }

		inline bool IsColliding() const { return (0 < m_collisionCount); }
		
		virtual MATRIX GetColliderMatrix() = 0;

	protected:
		inline Transform* GetTransform() { return m_transform; }
		inline CollisionSystem* GetCollisionSystem() { return m_collisionSystem; }
		inline void AddCollisionCount() { ++m_collisionCount; }
		inline void SubCollisionCount();
		
	private:
		eDimensionType m_dimension;
		bool m_isTriggerMode;

		//Transform을 많이 사용하므로 아예 주소를 받아 놓는다.
		Transform* m_transform;
		CollisionSystem* m_collisionSystem;

		int m_collisionCount;
	};

	inline void Collider::SubCollisionCount()
	{
		--m_collisionCount;
		if (m_collisionCount < 0)
		{
			m_collisionCount = 0;
			ASSERT_DEBUG(false, "Collision Count가 음수로 내려갔습니다.");
		}
	}
}


