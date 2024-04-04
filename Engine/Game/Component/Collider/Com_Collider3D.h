#pragma once
#include "Engine/Game/Component/Collider/iCollider.h"
#include "Engine/define_Enum.h"

namespace physx
{
	class PxShape;
	class PxActor;
}

namespace ehw
{
	enum class FreezeRotationFlag : uint8
	{
		ROTATION_X = (1 << 1),
		ROTATION_Y = (1 << 2),
		ROTATION_Z = (1 << 4),
		END
	};

	class Com_Collider3D : public iCollider
	{
	public:
		Com_Collider3D();
		//Com_Collider3D(const Com_Collider3D& _collider);
		CLONE_DISABLE(Com_Collider3D);
		
		virtual ~Com_Collider3D();

		void OnCollisionEnter(Com_Collider3D* const _collider, const float3& _contactPoint);
		void OnCollisionStay(Com_Collider3D* const _collider, const float3& _contactPoint);
		void OnCollisionExit(Com_Collider3D* const _collider);

		void OnTriggerEnter(Com_Collider3D* const _collider);
		void OnTriggerStay(Com_Collider3D* const _collider);
		void OnTriggerExit(Com_Collider3D* const _collider);

		eResult Serialize_Json(JsonSerializer* _ser) const override { return eResult(); };
		eResult DeSerialize_Json(const JsonSerializer* _ser) override { return eResult(); };

		void OnEnable() override;
		void OnDisable() override;

		void CollisionUpdate();
		void CollisionLateUpdate();
		
		void FinalUpdate() override { ASSERT(false, "미구현"); }; // 구현 안함

		inline void DestroyShape() { _shape = nullptr; };

		inline bool IsTrigger() const { return _isTrigger; };

		MATRIX GetColliderMatrix() override { return MATRIX{}; };
		
		void SetTrigger();

		float3 GetWorldScale() const;
		MATRIX GetWorldMatrix() const;

		inline void SetType(eCollider3D_Shape type, bool isStatic = false);
		inline eCollider3D_Shape GetCollider3DType() const { return _type; }

		MATRIX	getWorldMatrix() const { return _worldMatrix; }
		Vector3 getWorldPosition() const { return _worldMatrix.Translation(); }
		Vector3 getWorldScale() const { return _offsetScale; }

		void	SetCenter(Vector3 offset) { _offsetPosition = offset; }
		Vector3 GetCenter() const { return _offsetPosition; }

		void	SetSize(Vector3 offset);
		Vector3 GetSize() const { return _offsetScale; }

		inline bool isTrigger() const { return _isTrigger; }
		void setTrigger(bool enable);

		//씬 변경시 dontdestroy에 의해 제거되지 않고 씬만 변경 되었을 경우
		void SceneChanged();
		physx::PxShape* getShape() const { return _shape; }
		void			EnableGravity(bool enable);
		bool			isGravityEnabled() const { return _isGravityEnabled; }
		bool			isStatic() const { return _isStatic; }

		bool		isOverlapping(GameObject* other, Vector3* outDistance);
		GameObject* getOverlappingObject() const { return _otherOverlapping; }

		void setFreezeRotation(EnumFlags<FreezeRotationFlag> flag);
		bool hasFlag(FreezeRotationFlag flag) const;

		void AddForce(const float3& force);
		void ClearForce();

		void setEnableDebugDraw(bool enable) { _enableDraw = enable; }
		bool isDrawDebug() const { return _enableDraw; }

		void  SetMass(float mass);
		inline float GetMass() const { return _mass; };

		void SetRestitution(float restitution);
		inline float GetRestitution() const { return _restitution; }

		void SetVelocity(const float3& _velocity);
		float3 GetVelocity() const;

		void SetStaticFriction(float staticFriction);
		inline float GetStaticFriction() const { return _staticFriction; }

		void SetDynamicFriction(float dynamicFriction);
		inline float GetDynamicFriction() const { return _dynamicFriction; }

		void SetLimitVelocity(float maxVelocity);
		inline float GetLimitVelocity() const { return _maxVelocity; }

		void SetKinematic(bool enable);

		//Start 호출 되기 전에만 동작함.
		void SetSyncScaleToTransfom(bool enable) { _syncScaleToTransform = enable; }

	private:
		void SyncPhysX();
		void FetchPhysX();
		void UpdateMatrix();
		inline void destroyShape() { _shape = nullptr; }

	private:
		eCollider3D_Shape _type;
		float3		 _offsetPosition;
		float3		 _offsetScale;
		UINT32		 _collisionCount;

		MATRIX		 _localMatrix;
		MATRIX		 _worldMatrix;

		float		 _mass;
		float		 _restitution;
		float		 _staticFriction;
		float		 _dynamicFriction;
		float		 _maxVelocity;


		//renderer::WireFrameCB						_wireFrameData;

		physx::PxShape* _shape;
		bool			_isTrigger;
		bool			_isGravityEnabled;
		bool			_isStatic;
		bool			_isKinematic;
		bool			_syncScaleToTransform;

		GameObject* _otherOverlapping;

		EnumFlags<FreezeRotationFlag> _freezeRotationFlag;

		bool _enableDraw;
	};

	inline void Com_Collider3D::SetType(eCollider3D_Shape type, bool isStatic)
	{
		_type = type;
		_isStatic = isStatic;
	}
}


