#pragma once
#include "Engine/Scene/Component/Collider/Collider.h"



namespace ehw
{
	class Com_Transform;
	class Collision2D;
	class Collider2D : public Collider
	{
	public:
		eCollider2D_Shape GetColliderShape() const { return m_collider2DShape; }

		Collider2D(eCollider2D_Shape _type);
		Collider2D(const Collider2D& _collider);
		CLONE_DISABLE(Collider2D);

		virtual ~Collider2D();

		eResult Serialize_Json(JsonSerializer* _ser) const override;
		eResult DeSerialize_Json(const JsonSerializer* _ser) override;

		void Init() override;
		void FrameEnd() override;

		//CollisionManager에서 호출. 충돌체 정보를 계산한다.
		inline void ColliderUpdate();
		virtual void UpdateShape() = 0;

		void FinalUpdate() final {}

	protected:
		inline Collision2D* GetCollision2DManager() { return m_col2dManager; };

	private:
		eCollider2D_Shape m_collider2DShape;
		Collision2D* m_col2dManager;
		bool m_isColliderUpdated;
	};

	inline void Collider2D::ColliderUpdate()
	{
		if (m_isColliderUpdated)
		{
			return;
		}
		m_isColliderUpdated = true;

		UpdateShape();
	}
}
