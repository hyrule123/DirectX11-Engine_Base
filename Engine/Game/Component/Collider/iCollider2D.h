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

		eResult Serialize_Json(JsonSerializer* _ser) const override;
		eResult DeSerialize_Json(const JsonSerializer* _ser) override;

		void Update() final;
		void FrameEnd() override;

		//CollisionManager에서 호출. 충돌체 정보를 계산한다.
		inline void ColliderUpdate();
		virtual void UpdateShape() = 0;

		void FinalUpdate() final {}

		void OnCollisionEnter(iCollider2D* const _collider);
		void OnCollisionStay(iCollider2D* const _collider);
		void OnCollisionExit(iCollider2D* const _collider);

		void OnTriggerEnter(iCollider2D* const _collider);
		void OnTriggerStay(iCollider2D* const _collider);
		void OnTriggerExit(iCollider2D* const _collider);

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
