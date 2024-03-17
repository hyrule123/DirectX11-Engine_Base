#pragma once
#include "Engine/Game/Component/Collider/iCollider.h"



namespace ehw
{
	class Com_Transform;
	class iCollider2D : public iCollider
	{
	public:
		eCollider2D_Shape GetColliderShape() const { return m_collider2DType; }


		iCollider2D(eCollider2D_Shape _type);
		iCollider2D(const iCollider2D& _collider);
		CLONE_DISABLE(iCollider2D);

		virtual ~iCollider2D();

		virtual eResult Serialize_Json(JsonSerializer* _ser) const override;
		virtual eResult DeSerialize_Json(const JsonSerializer* _ser) override;

		virtual void Awake() override;
		virtual void Update() final;
		virtual void FinalUpdate() override;

	private:
		eCollider2D_Shape m_collider2DType;
	};
}
