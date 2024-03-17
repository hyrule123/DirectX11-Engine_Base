#pragma once
#include "Engine/Game/Component/Collider/iCollider.h"

namespace physx
{
	class PxShape;
	class PxActor;
}

enum class FreezeRotationFlag : UINT8
{
	ROTATION_X = (1 << 1),
	ROTATION_Y = (1 << 2),
	ROTATION_Z = (1 << 4),

	END
};


namespace ehw
{
	class iCollider3D : public iCollider
	{
	public:
		iCollider3D();
		iCollider3D(const iCollider3D& _collider);
		CLONE_DISABLE(iCollider3D);
		
		virtual ~iCollider3D();

		virtual eResult Serialize_Json(JsonSerializer* _ser) const override;
		virtual eResult DeSerialize_Json(const JsonSerializer* _ser) override;

		//virtual void Init() override;
		//virtual void Update() override;
		virtual void FinalUpdate() override { ASSERT(false, "미구현"); }; // 구현 안함

	private:
	};
}


