#pragma once
#include <Engine/Game/Component/Component.h>

#include <Engine/Common.h>

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
		virtual void awake() override;

		void on_collision_enter(const s_ptr<Collider>& _collider, const float3& _contactPoint);
		void on_collision_stay(const s_ptr<Collider>& _collider, const float3& _contactPoint);
		void on_collision_exit(const s_ptr<Collider>& _collider);
		void on_trigger_enter(const s_ptr<Collider>& _collider);
		void on_trigger_stay(const s_ptr<Collider>& _collider);
		void on_trigger_exit(const s_ptr<Collider>& _collider);

		inline eDimensionType get_collider_type() const { return m_dimension; }
		inline bool is_trigger_mode() const { return m_isTriggerMode; }

		inline bool is_colliding() const { return (0 < m_collisionCount); }
		
		virtual MATRIX get_collider_matrix() = 0;

	protected:
		inline s_ptr<Transform> get_transform() { return m_transform.lock(); }
		inline CollisionSystem* get_collision_system() { return m_collisionSystem; }
		inline void add_collision_count() { ++m_collisionCount; }
		inline void sub_collision_count();
		
	private:
		eDimensionType m_dimension;
		bool m_isTriggerMode;

		//Transform을 많이 사용하므로 아예 주소를 받아 놓는다.
		w_ptr<Transform> m_transform;
		CollisionSystem* m_collisionSystem;

		int m_collisionCount;
	};

	inline void Collider::sub_collision_count()
	{
		--m_collisionCount;
		if (m_collisionCount < 0)
		{
			m_collisionCount = 0;
			ASSERT_DEBUG(false, "Collision Count가 음수로 내려갔습니다.");
		}
	}
}



