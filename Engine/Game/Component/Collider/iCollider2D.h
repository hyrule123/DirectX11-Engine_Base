#pragma once
#include "Engine/Game/Component/Collider/iCollider.h"



namespace ehw
{
	class Com_Transform;
	class iCollider2D : public iCollider
	{
	public:
		eCollider2D_Shape GetColliderShape() const { return m_collider2DShape; }

		iCollider2D(eCollider2D_Shape _type);
		iCollider2D(const iCollider2D& _collider);
		CLONE_DISABLE(iCollider2D);

		virtual ~iCollider2D();

		virtual eResult Serialize_Json(JsonSerializer* _ser) const override;
		virtual eResult DeSerialize_Json(const JsonSerializer* _ser) override;

		virtual void Update() final;
		virtual void FrameEnd() override;

		//CollisionManager에서 호출. 충돌체 정보를 계산한다.
		inline void ColliderUpdate();
		virtual void UpdateShape() = 0;

		virtual void FinalUpdate() final {}

		virtual void OnCollisionEnter(iCollider* const _collider, const Vector2 _contactPoint) override;
		virtual void OnCollisionStay(iCollider* const _collider, const Vector2 _contactPoint) override;
		virtual void OnCollisionExit(iCollider* const _collider) override;

		virtual void OnTriggerEnter(iCollider* const _collider) override;
		virtual void OnTriggerStay(iCollider* const _collider) override;
		virtual void OnTriggerExit(iCollider* const _collider) override;

	private:
		eCollider2D_Shape m_collider2DShape;
		bool m_isColliderUpdated;
	};

	inline void iCollider2D::ColliderUpdate()
	{
		if (m_isColliderUpdated)
		{
			return;
		}
		m_isColliderUpdated = true;

		UpdateShape();
	}
}
