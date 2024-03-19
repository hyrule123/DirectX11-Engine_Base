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

		virtual void Init() override;
		virtual void FinalUpdate() override;


		virtual void OnCollisionEnter(const std::shared_ptr<iCollider>& _collider, const Vector2 _contactPoint) override;
		virtual void OnCollisionStay(const std::shared_ptr<iCollider>& _collider, const Vector2 _contactPoint) override;
		virtual void OnCollisionExit(const std::shared_ptr<iCollider>& _collider) override;

		virtual void OnTriggerEnter(const std::shared_ptr<iCollider>& _collider) override;
		virtual void OnTriggerStay(const std::shared_ptr<iCollider>& _collider) override;
		virtual void OnTriggerExit(const std::shared_ptr<iCollider>& _collider) override;

	private:
		eCollider2D_Shape m_collider2DShape;
	};
}
