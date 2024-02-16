#pragma once
#include "iCollider.h"

namespace ehw
{
	class iCollider3D : public iCollider
	{
	public:
		iCollider3D();
		iCollider3D(const iCollider3D& _collider);
		CLONE_ABLE(iCollider3D);
		
		virtual ~iCollider3D();

		virtual eResult Serialize_Json(JsonSerializer* _ser) override;
		virtual eResult DeSerialize_Json(const JsonSerializer* _ser) override;

		//virtual void Init() override;
		//virtual void Update() override;
		virtual void InternalUpdate() override { ASSERT(false, "미구현"); }; // 구현 안함

		void SetType(eColliderType _type) { m_ComCategory = _type; }

	private:
		eColliderType m_ComCategory;
	};
}


