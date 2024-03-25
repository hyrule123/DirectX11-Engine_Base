#include "Engine/Game/Component/Collider/iCollider2D.h"

#include "Engine/Game/Component/Transform/Com_Transform.h"
#include "Engine/Game/Component/Script/iScript.h"

#include "Engine/Game/GameObject.h"

#include "Engine/Manager/CollisionManager.h"
#include "Engine/Manager/RenderManager.h"


namespace ehw
{
	iCollider2D::iCollider2D(eCollider2D_Shape _type)
		: iCollider(eDimensionType::_2D)
		, m_collider2DShape(_type)
	{
		CollisionManager::RegisterCollider2D(this);
	}

	iCollider2D::iCollider2D(const iCollider2D& _collider)
		: iCollider(_collider)
	{
	}

	iCollider2D::~iCollider2D()
	{
		//제거는 CollisionManager::Update에서 일괄적으로 진행됨
		//각자 소멸자에서 제거요청을 보낼경우 매번 vector를 순회돌면서 지워야해서 속도가 많이 느림
	}

	eResult iCollider2D::Serialize_Json(JsonSerializer* _ser) const
	{
		return eResult();
	}

	eResult iCollider2D::DeSerialize_Json(const JsonSerializer* _ser)
	{
		return eResult();
	}

	void iCollider2D::Init()
	{
	}


	void iCollider2D::FinalUpdate()
	{
		//float3 scale = mTransform->GetLocalScale();
		//scale *= float3(mSize.x, mSize.y, 1.0f);

		//Quaternion rotation = mTransform->GetLocalRotation();

		//float3 position = mTransform->GetLocalPosition();
		//float3 colliderPos = position + float3(mCenter.x, mCenter.y, 0.0f);
		//mPosition = colliderPos;

		//MATRIX scaleMatrix = MATRIX::CreateScale(scale);
		//MATRIX rotationMatrix;
		//rotationMatrix = MATRIX::CreateRotationX(rotation.x);
		//rotationMatrix *= MATRIX::CreateRotationY(rotation.y);
		//rotationMatrix *= MATRIX::CreateRotationZ(rotation.z);

		//MATRIX positionMatrix;
		//positionMatrix.Translation(float3(colliderPos.x, colliderPos.y, colliderPos.z));

		//MATRIX worldMatrix = scaleMatrix * rotationMatrix * positionMatrix;

		//tDebugMesh meshAttribute = {};
		//meshAttribute.position = float3(colliderPos.x, colliderPos.y, colliderPos.z);
		//meshAttribute.radius = mRadius;
		//meshAttribute.rotation = rotation;
		//meshAttribute.scale = scale;
		////meshAttribute.type = GetColli;

		//RenderManager::AddDebugMesh(meshAttribute);
	}
	void iCollider2D::OnCollisionEnter(iCollider* const _collider, const Vector2 _contactPoint)
	{
		//DEBUG_BREAK;
	}
	void iCollider2D::OnCollisionStay(iCollider* const _collider, const Vector2 _contactPoint)
	{
		//DEBUG_BREAK;
	}
	void iCollider2D::OnCollisionExit(iCollider* const _collider)
	{
		DEBUG_BREAK;
	}
	void iCollider2D::OnTriggerEnter(iCollider* const _collider)
	{
		//DEBUG_BREAK;
	}
	void iCollider2D::OnTriggerStay(iCollider* const _collider)
	{
		//DEBUG_BREAK;
	}
	void iCollider2D::OnTriggerExit(iCollider* const _collider)
	{
		DEBUG_BREAK;
	}
}
