#include "Engine/Game/Component/Collider/Com_Collider3D.h"

#include "Engine/Game/GameObject.h"
#include "Engine/Game/Component/Transform/Com_Transform.h"

#include "Engine/Manager/CollisionManager.h"
#include "Engine/Manager/Collision/MathUtil.h"

#include <PhysX/PxPhysicsAPI.h>

namespace ehw
{
	Com_Collider3D::Com_Collider3D()
		: iCollider{eDimensionType::_3D}
		, _type{ eCollider3D_Shape::Box }
		, _offsetScale{ 1.f, 1.f, 1.f }
		, _collisionCount{ 0 }
		, _shape{ nullptr }
		, _isTrigger{ false }
		, _isGravityEnabled(true)
		, _otherOverlapping{ nullptr }
		, _freezeRotationFlag{ FreezeRotationFlag::END }
		, _enableDraw{ true }
		, _mass(0.6f)
		, _restitution(0.f)
		, _syncScaleToTransform(true)
		, _staticFriction(180.f)
		, _dynamicFriction(180.f)
		, _maxVelocity(40.f)
		, _isStatic(false)
		, _isKinematic(false)
	{
	}

	Com_Collider3D::~Com_Collider3D()
	{
		if (_shape)
		{
			physx::PxActor* actor = _shape->getActor();
			actor->userData = nullptr;
			actor->release();
			_shape = nullptr;
		}
	}

	void Com_Collider3D::Start()
	{
		if (_syncScaleToTransform)
		{
			Com_Transform* tr = GetOwner()->GetComponent<Com_Transform>();
			_offsetScale *= tr->GetWorldScale();
		}
		float3 realScale = _offsetScale * 0.5f;


		if (_shape == nullptr)
		{
			if (_type == eCollider3D_Shape::Box)
			{
				CollisionManager::GetCollision3D().createActorCube(GetOwner(), realScale, &_shape, _isStatic);
			}
				
			else if (_type == eCollider3D_Shape::Sphere)
			{
				CollisionManager::GetCollision3D().createActorSphere(GetOwner(), realScale.x, &_shape, _isStatic);
			}
				
		}
		else if (_shape)
		{
			CollisionManager::GetCollision3D().changeGeometry(this, _shape, _type);
		}


		EnableGravity(_isGravityEnabled);
		SetMass(_mass);
		SetRestitution(_restitution);
		EnableGravity(_isGravityEnabled);
		SetStaticFriction(_staticFriction);
		SetDynamicFriction(_dynamicFriction);
		SetLimitVelocity(_maxVelocity);
		setFreezeRotation(_freezeRotationFlag.Get());
		SetKinematic(_isKinematic);

		if (_isStatic)
		{
			UpdateMatrix();
			SyncPhysX();
		}
	}




	//void Com_Collider3D::Render()
	//{
		//_positionBufferData.world = _worldMatrix;

		////const Camera& camera = *(renderer::cameras[(UINT)eSceneType::Play]);
		//_positionBufferData.view = Camera::GetGpuViewMatrix();
		//_positionBufferData.projection = Camera::GetGpuProjectionMatrix();
		//_positionBuffer->SetData(&_positionBufferData);
		//_positionBuffer->Bind(eShaderStage::ALL);

		//_wireFrameData.collisionCount = _collisionCount;
		//_wireFrameBuffer->SetData(&_wireFrameData);
		//_wireFrameBuffer->Bind(eShaderStage::PS);

		//_shader->Binds();
		//_mesh->Render(0u);

	//}

	void Com_Collider3D::CollisionUpdate()
	{
		ASSERT(_type != eCollider3D_Shape::END, "이상한 type");

		//static일 경우 scale 설정은 최초 한번만
		if (false == _isStatic)
		{
			UpdateMatrix();
			SyncPhysX();
		}
	}

	void Com_Collider3D::CollisionLateUpdate()
	{
		FetchPhysX();
	}



	void Com_Collider3D::SetSize(float3 offset)
	{
		_offsetScale = offset;

		if (_type == eCollider3D_Shape::Sphere)
		{
			_offsetScale.x = _offsetScale.y = _offsetScale.z;

			if (_shape)
			{
				float3 realScale = GetOwner()->GetComponent<Com_Transform>()->GetWorldScale() * _offsetScale * 0.5f;
				physx::PxGeometryHolder holder{ _shape->getGeometry() };
				holder.sphere().radius = realScale.x;
				_shape->setGeometry(holder.sphere());
			}
		}
		else
		{
			if (_shape)
			{
				float3 realScale = GetOwner()->GetComponent<Com_Transform>()->GetWorldScale() * _offsetScale * 0.5f;
				physx::PxGeometryHolder holder{ _shape->getGeometry() };
				holder.box().halfExtents = MathUtil::ToPxVec3(realScale);
				_shape->setGeometry(holder.box());
			}
		}
	}

	void Com_Collider3D::OnCollisionEnter(Com_Collider3D* other, const float3& collisionPosition)
	{
		++_collisionCount;

		const auto& scripts = GetOwner()->GetScripts();
		for (auto& script : scripts)
		{
			script->OnCollisionEnter(other, collisionPosition);
		}
	}

	void Com_Collider3D::OnCollisionStay(Com_Collider3D* other, const float3& collisionPosition)
	{
		const auto& scripts = GetOwner()->GetScripts();
		for (auto& script : scripts)
		{
			script->OnCollisionStay(other, collisionPosition);
		}
	}

	void Com_Collider3D::OnCollisionExit(Com_Collider3D* other)
	{
		--_collisionCount;
		const auto& scripts = GetOwner()->GetScripts();
		for (auto& script : scripts)
		{
			script->OnCollisionExit(other);
		}
	}

	void Com_Collider3D::OnTriggerEnter(Com_Collider3D* other)
	{
		_otherOverlapping = other->GetOwner();
		const auto& scripts = GetOwner()->GetScripts();
		for (auto& script : scripts)
		{
			script->OnTriggerEnter(other);
		}

		//_wireFrameData.isTrigger = true;
	}

	void Com_Collider3D::OnTriggerExit(Com_Collider3D* other)
	{
		const auto& scripts = GetOwner()->GetScripts();
		for (auto& script : scripts)
		{
			script->OnTriggerExit(other);
		}

		_otherOverlapping = nullptr;
		//_wireFrameData.isTrigger = false;
	}

	void Com_Collider3D::setTrigger(bool enable)
	{
		_isTrigger = enable;
		assert(_type != eCollider3D_Shape::END);

		physx::PxShapeFlags flags = _shape->getFlags();

		if (enable)
		{
			flags &= ~physx::PxShapeFlag::eSIMULATION_SHAPE;
			flags |= physx::PxShapeFlag::eTRIGGER_SHAPE;
		}
		else
		{
			flags &= ~physx::PxShapeFlag::eTRIGGER_SHAPE;
			flags |= physx::PxShapeFlag::eSIMULATION_SHAPE;
		}

		_shape->setFlags(flags);
	}

	void Com_Collider3D::SceneChanged()
	{
		if (_shape)
		{
			physx::PxActor* actor = _shape->getActor();
			actor->userData = nullptr;
			actor->release();
			_shape = nullptr;
		}
		SetType(GetCollider3DType(), _isStatic);
	}

	void Com_Collider3D::EnableGravity(bool enable)
	{
		_isGravityEnabled = enable;

		if (_shape)
		{
			physx::PxActor* actor = _shape->getActor();

			auto flags = actor->getActorFlags();
			if (_isGravityEnabled)
				flags &= ~physx::PxActorFlag::eDISABLE_GRAVITY;
			else
				flags |= physx::PxActorFlag::eDISABLE_GRAVITY;

			actor->setActorFlags(flags);
		}
	}

	bool Com_Collider3D::isOverlapping(GameObject* other, float3* outDistance)
	{
		physx::PxVec3 direction{};
		physx::PxF32  depth{};

		Com_Collider3D* otherCollider = other->GetComponent<Com_Collider3D>();
		const bool		  isPenetrating = physx::PxGeometryQuery::computePenetration(direction, depth,
			_shape->getGeometry(),
			_shape->getActor()->getGlobalPose(),
			otherCollider->getShape()->getGeometry(),
			otherCollider->getShape()->getActor()->getGlobalPose());

		if (outDistance && isPenetrating)
		{
			const physx::PxVec3 result = direction * depth;
			*outDistance = MathUtil::Tofloat3(result);
		}

		return isPenetrating;
	}

	void Com_Collider3D::setFreezeRotation(EnumFlags<FreezeRotationFlag> flag)
	{
		_freezeRotationFlag = flag;

		if (_shape)
		{
			physx::PxActor* actor = _shape->getActor();
			assert(actor);

			physx::PxRigidDynamic* rigidActor = actor->is<physx::PxRigidDynamic>();
			if (rigidActor == nullptr)
				return;

			physx::PxRigidDynamicLockFlags pxFlag{};

			if (_freezeRotationFlag & FreezeRotationFlag::ROTATION_X)
				pxFlag |= physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X;
			if (_freezeRotationFlag & FreezeRotationFlag::ROTATION_Y)
				pxFlag |= physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y;
			if (_freezeRotationFlag & FreezeRotationFlag::ROTATION_Z)
				pxFlag |= physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z;


			rigidActor->setRigidDynamicLockFlags(pxFlag);

			rigidActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_X, true);
			rigidActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, true);
		}


	}

	bool Com_Collider3D::hasFlag(FreezeRotationFlag flag) const
	{
		return false;
	}

	void Com_Collider3D::AddForce(const float3& force)
	{
		if (_shape)
		{
			physx::PxActor* actor = _shape->getActor();
			if (actor)
			{
				physx::PxRigidBody* rigidActor = actor->is<physx::PxRigidBody>();
				if (rigidActor)
				{
					rigidActor->addForce(MathUtil::ToPxVec3(force), physx::PxForceMode::eACCELERATION);
					//Com_Transform* transform = GetOwner()->GetComponent<Com_Transform>();
					//physx::PxRigidBodyExt::addForceAtPos(*rigidActor, MathUtil::ToPxVec3(force), MathUtil::ToPxVec3(transform->GetWorldPosition()));
				}
			}
		}
	}

	void Com_Collider3D::AddForce(const float3& force)
	{
		ASSERT(false, "미구현");
	}

	void Com_Collider3D::ClearForce()
	{
		if (_shape)
		{
			physx::PxActor* actor = _shape->getActor();
			if (actor)
			{
				physx::PxRigidBody* rigidActor = actor->is<physx::PxRigidBody>();
				if (rigidActor)
				{
					rigidActor->clearForce();
				}
			}
		}
	}


	void Com_Collider3D::SetMass(float mass)
	{
		_mass = mass;
		if (_shape)
		{
			physx::PxRigidBody* rigidBody = _shape->getActor()->is<physx::PxRigidBody>();
			if (rigidBody)
			{
				rigidBody->setMass(_mass);
			}
		}
	}

	void Com_Collider3D::SetRestitution(float restitution)
	{
		_restitution = restitution;
		if (_shape)
		{
			physx::PxMaterial* mtrl{};
			_shape->getMaterials(&mtrl, 1u, 0u);
			if (mtrl)
			{
				mtrl->setRestitution(_restitution);
			}
		}
	}

	void Com_Collider3D::SetVelocity(const float3& _velocity)
	{
		if (_shape)
		{
			physx::PxRigidDynamic* dynamicRigid = _shape->getActor()->is<physx::PxRigidDynamic>();
			if (dynamicRigid)
			{
				dynamicRigid->setLinearVelocity(MathUtil::ToPxVec3(_velocity));
			}
		}
	}

	float3 Com_Collider3D::GetVelocity() const
	{
		float3 retVec{};

		if (_shape)
		{
			physx::PxRigidDynamic* dynamicRigid = _shape->getActor()->is<physx::PxRigidDynamic>();
			if (dynamicRigid)
			{
				retVec = MathUtil::Tofloat3(dynamicRigid->getLinearVelocity());
			}
		}

		return retVec;
	}

	void Com_Collider3D::SetStaticFriction(float staticFriction)
	{
		_staticFriction = staticFriction;
		if (_shape)
		{
			physx::PxMaterial* mtrl{};
			_shape->getMaterials(&mtrl, 1u, 0u);
			if (mtrl)
			{
				mtrl->setStaticFriction(_staticFriction);
			}
		}
	}


	void Com_Collider3D::SetDynamicFriction(float dynamicFriction)
	{
		_dynamicFriction = dynamicFriction;
		if (_shape)
		{
			physx::PxMaterial* mtrl{};
			_shape->getMaterials(&mtrl, 1u, 0u);
			if (mtrl)
			{
				mtrl->setDynamicFriction(_dynamicFriction);
			}
		}
	}

	void Com_Collider3D::SetLimitVelocity(float maxVelocity)
	{
		_maxVelocity = maxVelocity;
		if (_shape)
		{
			physx::PxRigidDynamic* dynamicRigid = _shape->getActor()->is<physx::PxRigidDynamic>();
			if (dynamicRigid)
			{
				dynamicRigid->setMaxLinearVelocity(maxVelocity);
			}
		}
	}

	void Com_Collider3D::SetKinematic(bool enable)
	{
		if (_shape)
		{
			physx::PxRigidBody* rigid = _shape->getActor()->is<physx::PxRigidBody>();
			if (rigid)
			{
				rigid->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, enable);
			}
		}
	}



	void Com_Collider3D::SyncPhysX()
	{
		physx::PxActor* actor = _shape->getActor();
		assert(actor);
		physx::PxRigidActor* rigidActor = actor->is<physx::PxRigidActor>();
		assert(rigidActor);

		physx::PxTransformT<float> transform{};
		memcpy(&transform.p, _worldMatrix.m[3], sizeof(float) * 3);

		Quaternion q = GetOwner()->GetComponent<Com_Transform>()->GetWorldRotation();
		transform.q = MathUtil::ToPxQuat(q);

		rigidActor->setGlobalPose(transform);
	}
	void Com_Collider3D::FetchPhysX()
	{
		physx::PxActor* actor = _shape->getActor();
		assert(actor);

		physx::PxRigidActor* rigidActor = actor->is<physx::PxRigidActor>();
		assert(rigidActor);

		physx::PxTransform worldTransform = rigidActor->getGlobalPose();

		//Debug
		if (GetOwner()->GetName() == "Wolf")
		{
			static Quaternion prevQuat{};
			Quaternion curQuat = MathUtil::ToQuaternion(worldTransform.q);
			prevQuat = curQuat;

			float3 angle = MathUtil::ToQuaternion(worldTransform.q).ToEuler() * g_radianToDegreeFactor;
				
			int iangle = (int)std::floorf(angle.x);
			if (0 == iangle)
			{
				int a = 3;
			}
			else if (-180 == iangle)
			{
				int a = 3;
			}
		}

		if (false == _isStatic)
		{
			MATRIX before = MATRIX::CreateScale(_offsetScale).Invert() * _worldMatrix;
			MATRIX after = MathUtil::ToMATRIX(physx::PxMat44(worldTransform));
			MATRIX diff = before.Invert() * after;

			Quaternion diffQuat = Quaternion::CreateFromRotationMatrix(diff);

			float3 diffPos = diff.Translation();

			if (GetOwner()->GetName() == "childObj")
			{
				int a = 3;
			}

			Com_Transform* tr = GetOwner()->GetComponent<Com_Transform>();
			tr->SetWorldRotation(diffQuat);
			tr->SetWorldPosition(diffPos);
			//GetOwner()->GetComponent<Com_Transform>()->FetchPhysX(diffQuat, diffPos);
		}
	}
	void Com_Collider3D::UpdateMatrix()
	{
		//이동은 크기에 영향을 받으므로 우선 크기 정보를 반영해서 MATRIX를 만들어줘야 한다.
		_localMatrix = MATRIX::CreateScale(_offsetScale);
		_localMatrix *= MATRIX::CreateTranslation(_offsetPosition);

		//트랜스폼의 크기 정보는 제거
		const float3 objectScale = GetOwner()->GetComponent<Com_Transform>()->GetWorldScale();
		const MATRIX objectScaleInvMatrix = MATRIX::CreateScale(objectScale).Invert();

		const MATRIX  objectWorldMatrix = GetOwner()->GetComponent<Com_Transform>()->GetWorldMatrix();
		// 충돌체 상대행렬 * 오브젝트 월드 크기 역행렬 * 오브젝트 월드 행렬(크기 * 회전 * 이동)
		//트랜스폼의 Scale은 반영하지 않는다
		_worldMatrix = _localMatrix * objectScaleInvMatrix * objectWorldMatrix;
	}
} // namespace ehw
