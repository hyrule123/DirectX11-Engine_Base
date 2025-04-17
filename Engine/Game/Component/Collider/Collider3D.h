#pragma once
#include "Engine/Game/Component/Collider/Collider.h"
#include "Engine/define_Enum.h"

#include "Engine/Game/Collision/PhysX.h"

namespace core
{
	class Transform;
	class Collision3D;
	class Collider3D : public Collider
	{
		CLONE_DISABLE(Collider3D);
	public:
		Collider3D(const std::string_view key, eCollider3DType _col3dType);
		
		virtual ~Collider3D();

		eResult serialize_json(JsonSerializer* _ser) const override { return eResult(); };
		eResult deserialize_json(const JsonSerializer* _ser) override { return eResult(); };

		void init() override;
		void Awake() override;

		MATRIX GetColliderMatrix() override { return MATRIX{}; };

		//Start 호출 되기 전에만 동작함.
		void SetSyncScaleToTransfom(bool enable) { m_isSyncScaleToTransform = enable; }

	protected:
		inline bool IsSyncScaleToTransform() const { return m_isSyncScaleToTransform; }
		inline Collision3D* GetCollision3D() { return m_col3dMgr; }

	private:
		eCollider3DType				m_colliderType;

		bool						m_isSyncScaleToTransform;

		Collision3D*				m_col3dMgr;
	};
}


