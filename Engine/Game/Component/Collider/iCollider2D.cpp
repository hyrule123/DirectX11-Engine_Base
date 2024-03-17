#include "Engine/Game/Component/Collider/iCollider2D.h"

#include "Engine/Game/Component/Transform/Com_Transform.h"
#include "Engine/Game/Component/Script/iScript.h"

#include "Engine/Game/GameObject.h"

#include "Engine/Manager/CollisionManager.h"
#include "Engine/Manager/RenderManager.h"

namespace ehw
{
	iCollider2D::iCollider2D(eCollider2D_Shape _type)
		: iCollider(eColliderType::None)
	{
	}

	iCollider2D::iCollider2D(const iCollider2D& _collider)
		: iCollider(_collider)
	{
	}

	iCollider2D::~iCollider2D()
	{
	}

	eResult iCollider2D::Serialize_Json(JsonSerializer* _ser) const
	{
		return eResult();
	}

	eResult iCollider2D::DeSerialize_Json(const JsonSerializer* _ser)
	{
		return eResult();
	}

	void iCollider2D::Awake()
	{
	}

	void iCollider2D::Update()
	{
		CollisionManager::EnqueueCollider2D(shared_from_this_T<iCollider2D>());
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
}
