#pragma once
#include "Engine/Game/Component/iComponent.h"

#include "Engine/CommonStruct.h"

#include <unordered_set>

namespace ehw
{
	class iCollider
		: public Component<iCollider, eComponentCategory::Collider>
	{
	public:
		iCollider(eColliderType _type);
		iCollider(const iCollider& _collider) = default;
		virtual ~iCollider();

		eColliderType GetColliderType() const { return m_colliderType; }

		virtual void OnCollisionEnter(const std::shared_ptr<iCollider>& _collider, const Vector2 _contactPoint) = 0;
		virtual void OnCollisionStay(const std::shared_ptr<iCollider>& _collider, const Vector2 _contactPoint) = 0;
		virtual void OnCollisionExit(const std::shared_ptr<iCollider>& _collider) = 0;

		virtual void OnTriggerEnter(const std::shared_ptr<iCollider>& _collider, const Vector2 _contactPoint) = 0;
		virtual void OnTriggerStay(const std::shared_ptr<iCollider>& _collider, const Vector2 _contactPoint) = 0;
		virtual void OnTriggerExit(const std::shared_ptr<iCollider>& _collider) = 0;

		void CollisionUpdate();

	private:
		void Contacted(const std::shared_ptr<iCollider>& _col, const Vector2 _contactPoint);

		//Collider를 가진 모든 GameObject에 대해 무조건 호출. 충돌이 끝났을 경우 OnCollisionExit 호출.
		
	private:
		eColliderType m_colliderType;
		
		std::unordered_set<std::shared_ptr<iCollider>> m_prevContacts;
		std::unordered_set<std::shared_ptr<iCollider>> m_curContacts;
	};
}


