
#include "iCollider2D.h"



#include "../../../Game/GameObject.h"

#include "../../../Manager/RenderManager.h"


#include "../Transform/Com_Transform.h"
#include "../Script/iScript.h"

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
		: iCollider(_collider.m_ComCategory)
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

	eResult iCollider2D::Serialize(JsonSerializer& _ser)
	{
		return eResult();
	}

	eResult iCollider2D::DeSerialize(const JsonSerializer& _ser)
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

	void iCollider2D::InternalUpdate()
	{
		float3 scale = mTransform->GetRelativeScale();
		scale *= float3(mSize.x, mSize.y, 1.0f);

		float3 rotation = mTransform->GetRelativeRotXYZ();

		float3 position = mTransform->GetRelativePos();
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
		meshAttribute.rotatation = rotation;
		meshAttribute.scale = scale;
		meshAttribute.type = m_ComCategory;

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
