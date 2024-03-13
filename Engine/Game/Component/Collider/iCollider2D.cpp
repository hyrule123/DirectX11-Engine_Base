#include "Engine/Game/Component/Collider/iCollider2D.h"

#include "Engine/Game/Component/Transform/Com_Transform.h"
#include "Engine/Game/Component/Script/iScript.h"

#include "Engine/Game/GameObject.h"

#include "Engine/Manager/RenderManager.h"



namespace ehw
{
	uint iCollider2D::gColliderNumber = 0;
	iCollider2D::iCollider2D()
		: iCollider(eColliderType::None)
		, mTransform(nullptr)
		, mSize(float2::One)
		, mCenter(float2::Zero)
		, mbTrigger(false)
		, mID(0)
		, mRadius(0.0f)
	{
		mID = gColliderNumber++;
	}

	iCollider2D::iCollider2D(const iCollider2D& _collider)
		: iCollider(_collider)
		, mTransform(nullptr)
		, mSize(_collider.mSize)
		, mCenter(_collider.mCenter)
		, mbTrigger(false)
		, mID(0)
		, mRadius(_collider.mRadius)
	{
		mID = gColliderNumber++;
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
		mTransform = GetOwner()->GetComponent<Com_Transform>();
	}

	void iCollider2D::Update()
	{
	}

	void iCollider2D::LateUpdate()
	{
		float3 scale = mTransform->GetLocalScale();
		scale *= float3(mSize.x, mSize.y, 1.0f);

		Quaternion rotation = mTransform->GetLocalRotation();

		float3 position = mTransform->GetLocalPosition();
		float3 colliderPos = position + float3(mCenter.x, mCenter.y, 0.0f);
		mPosition = colliderPos;

		MATRIX scaleMatrix = MATRIX::CreateScale(scale);
		MATRIX rotationMatrix;
		rotationMatrix = MATRIX::CreateRotationX(rotation.x);
		rotationMatrix *= MATRIX::CreateRotationY(rotation.y);
		rotationMatrix *= MATRIX::CreateRotationZ(rotation.z);

		MATRIX positionMatrix;
		positionMatrix.Translation(float3(colliderPos.x, colliderPos.y, colliderPos.z));

		MATRIX worldMatrix = scaleMatrix * rotationMatrix * positionMatrix;

		tDebugMesh meshAttribute = {};
		meshAttribute.position = float3(colliderPos.x, colliderPos.y, colliderPos.z);
		meshAttribute.radius = mRadius;
		meshAttribute.rotation = rotation;
		meshAttribute.scale = scale;
		//meshAttribute.type = GetColli;

		RenderManager::AddDebugMesh(meshAttribute);
	}


	void iCollider2D::OnCollisionEnter(const std::shared_ptr<iCollider2D>& _collider)
	{
		const GameObject::Scripts& scripts = GetOwner()->GetScripts();
		for (const std::shared_ptr<iScript>& script : scripts)
		{
			script->OnCollisionEnter(_collider);
		}
	}

	void iCollider2D::OnCollisionStay(const std::shared_ptr<iCollider2D>& _collider)
	{
		const GameObject::Scripts& scripts = GetOwner()->GetScripts();
		for (const std::shared_ptr<iScript>& script : scripts)
		{
			script->OnCollisionStay(_collider);
		}
	}

	void iCollider2D::OnCollisionExit(const std::shared_ptr<iCollider2D>& _collider)
	{
		const GameObject::Scripts& scripts = GetOwner()->GetScripts();
		for (const std::shared_ptr<iScript>& script : scripts)
		{
			script->OnCollisionExit(_collider);
		}
	}

	void iCollider2D::OnTriggerEnter(const std::shared_ptr<iCollider2D>& _collider)
	{
		const GameObject::Scripts& scripts = GetOwner()->GetScripts();
		for (const std::shared_ptr<iScript>& script : scripts)
		{
			script->OnTriggerEnter(_collider);
		}
	}

	void iCollider2D::OnTriggerStay(const std::shared_ptr<iCollider2D>& _collider)
	{
		const GameObject::Scripts& scripts = GetOwner()->GetScripts();
		for (const std::shared_ptr<iScript>& script : scripts)
		{
			script->OnTriggerStay(_collider);
		}
	}

	void iCollider2D::OnTriggerExit(const std::shared_ptr<iCollider2D>& _collider)
	{
		const GameObject::Scripts& scripts = GetOwner()->GetScripts();
		for (const std::shared_ptr<iScript>& script : scripts)
		{
			script->OnTriggerExit(_collider);
		}
	}

}
