#pragma once
#include <Engine/Game/Component/Collider/Collider.h>



namespace core
{
	class Transform;
	class Collision2D;
	class Collider2D : public Collider
	{
		CLASS_INFO(Collider2D, Collider);
		CLONE_DISABLE(Collider2D);
	public:
		Collider2D(const std::string_view key, eCollider2D_Shape _type);
		Collider2D(const Collider2D& _other);

		virtual ~Collider2D();

		eResult serialize_json(JsonSerializer* _ser) const override;
		eResult deserialize_json(const JsonSerializer* _ser) override;

		void init() override;
		void frame_end() override;

		//CollisionManager에서 호출. 충돌체 정보를 계산한다.
		inline void collider_update();
		virtual void update_shape() = 0;

		void final_update() final {}

		eCollider2D_Shape get_collider_shape() const { return m_collider_shape_2D; }

	protected:
		inline Collision2D* get_collision2D_manager() { return m_collision_manager_2D; };

	private:
		eCollider2D_Shape m_collider_shape_2D;
		Collision2D* m_collision_manager_2D;
		bool m_b_collider_updated;
	};

	inline void Collider2D::collider_update()
	{
		if (m_b_collider_updated)
		{
			return;
		}
		m_b_collider_updated = true;

		update_shape();
	}
}
