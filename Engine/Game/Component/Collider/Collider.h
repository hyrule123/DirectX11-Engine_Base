#pragma once
#include "Engine/Game/Component/Component.h"

#include "Engine/Common.h"

#include <unordered_set>

namespace core
{
	class Transform;
	class CollisionSystem;
	
	class Collider
		: public Component
	{
		CLASS_INFO(Collider, Component);
		BASE_COMPONENT(eComponentOrder::Collider);

		friend class Collision2D;
		friend class Collision3D;
		friend class ContactPair;
	public:
		Collider(const std::string_view key, eDimensionType _dimension);
		Collider(const Collider& _collider) = default;
		virtual ~Collider();

		virtual void init() override;
		virtual void Awake() override;

		void OnCollisionEnter(const s_ptr<Collider>& _collider, const float3& _contactPoint);
		void OnCollisionStay(const s_ptr<Collider>& _collider, const float3& _contactPoint);
		void OnCollisionExit(const s_ptr<Collider>& _collider);
		void OnTriggerEnter(const s_ptr<Collider>& _collider);
		void OnTriggerStay(const s_ptr<Collider>& _collider);
		void OnTriggerExit(const s_ptr<Collider>& _collider);

		inline eDimensionType GetColliderType() const { return m_dimension; }
		inline bool IsTriggerMode() const { return m_isTriggerMode; }

		inline bool IsColliding() const { return (0 < m_collisionCount); }
		
		virtual MATRIX GetColliderMatrix() = 0;

	protected:
		inline s_ptr<Transform> GetTransform() { return m_transform.lock(); }
		inline CollisionSystem* GetCollisionSystem() { return m_collisionSystem; }
		inline void AddCollisionCount() { ++m_collisionCount; }
		inline void SubCollisionCount();
		
	private:
		eDimensionType m_dimension;
		bool m_isTriggerMode;

		//Transform을 많이 사용하므로 아예 주소를 받아 놓는다.
		w_ptr<Transform> m_transform;
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



