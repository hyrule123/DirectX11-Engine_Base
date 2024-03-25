#pragma once
#include "Engine/Game/Component/iComponent.h"

#include "Engine/CommonStruct.h"

#include <unordered_set>

namespace ehw
{
	class Com_Transform;
	class iCollider
		: public Component<iCollider, eComponentCategory::Collider>
	{
		friend class Collision2D;
		friend class Collision3D;
	public:
		iCollider(eDimensionType _dimension);
		iCollider(const iCollider& _collider) = default;
		virtual ~iCollider();

		virtual void Awake() override;

		inline eDimensionType GetColliderType() const { return m_dimension; }
		inline bool IsTriggerMode() const { return m_isTriggerMode; }

		virtual void OnCollisionEnter(iCollider* const _collider, const Vector2 _contactPoint) {}
		virtual void OnCollisionStay(iCollider* const _collider, const Vector2 _contactPoint) {}
		virtual void OnCollisionExit(iCollider* const _collider) {}

		virtual void OnTriggerEnter(iCollider* const _collider) {}
		virtual void OnTriggerStay(iCollider* const _collider) {}
		virtual void OnTriggerExit(iCollider* const _collider) {}

		inline bool IsColliding() const { return m_isColliding; }

	protected:
		inline Com_Transform* GetMyTransform() { return m_transform; }
		
	private:
		const MATRIX& GetWorldMatrix();
		inline void SetColliding(bool _isColliding) { m_isColliding = _isColliding; }

		eDimensionType m_dimension;
		bool m_isTriggerMode;

		//Transform을 많이 사용하므로 아예 주소를 받아 놓는다.
		Com_Transform* m_transform;

		bool m_isColliding;
	};
}


