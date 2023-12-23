#pragma once
#include "iCollider.h"


namespace ehw
{
	class Com_Transform;
	class iCollider2D : public iCollider
	{

	public:
		iCollider2D();
		iCollider2D(const iCollider2D& _collider);
		CLONE(iCollider2D);

		virtual ~iCollider2D();

		virtual eResult SaveJson(Json::Value* _pJVal) override;
		virtual eResult LoadJson(const Json::Value* _pJVal) override;

		virtual void Awake() override;
		virtual void Update() override;
		virtual void InternalUpdate() override;


		virtual void OnCollisionEnter(const std::shared_ptr<iCollider2D>& _collider);
		virtual void OnCollisionStay(const std::shared_ptr<iCollider2D>& _collider);
		virtual void OnCollisionExit(const std::shared_ptr<iCollider2D>& _collider);

		virtual void OnTriggerEnter(const std::shared_ptr<iCollider2D>& _collider);
		virtual void OnTriggerStay(const std::shared_ptr<iCollider2D>& _collider);
		virtual void OnTriggerExit(const std::shared_ptr<iCollider2D>& _collider);

		void SetType(eColliderType _type) { m_ComCategory = _type; }
		void SetSize(float2 _size) { mSize = _size; }
		void SetCenter(float2 _center) { mCenter = _center; }
		void SetRadius(float _radius) { mRadius = _radius; }
		bool IsTriiger() const { return mbTrigger; }
		uint GetID() const { return mID; }
		float3 GetRelativePos() const { return mPosition; }
		float2 GetSize() const { return mSize; }

	private:
		static uint gColliderNumber;
		uint mID; 
		eColliderType m_ComCategory;
		std::shared_ptr<Com_Transform> mTransform;

		float2 mSize;
		float2 mCenter;
		float3 mPosition;
		float mRadius;
		bool mbTrigger;
	};
}
