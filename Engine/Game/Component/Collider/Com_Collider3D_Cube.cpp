#include "Engine/Game/Component/Collider/Com_Collider3D_Cube.h"

#include "Engine/Game/Component/Transform/Com_Transform.h"

#include "Engine/Game/Collision/Collision3D.h"

#include "Engine/Game/Collision/PhysXInstance.h"

#include "Engine/Game/GameObject.h"
#include "Engine/Game/iScene.h"

namespace ehw
{
	Com_Collider3D_Cube::Com_Collider3D_Cube()
		: Com_Collider3D_Rigid(eCollider3D_Shape::Cube)
		, m_pxBoxGeometry(1.f)
		, m_offsetPosition(physx::PxIdentity)
		, m_offsetScale(1.f)
	{
	}
	Com_Collider3D_Cube::~Com_Collider3D_Cube()
	{
	}

	void Com_Collider3D_Cube::Awake()
	{
		Com_Collider3D_Rigid::Awake();

		Com_Transform* tr = GetTransform();

		m_pxBoxGeometry.halfExtents = (tr->GetWorldScale() * m_offsetScale) * 0.5f;

		Collision3D* col3dmgr = GetCollision3D();
		if (nullptr == col3dmgr)
		{
			ASSERT_DEBUG(nullptr, "Collision3D Manager이 존재하지 않음.");
			return;
		}

		physx::PxMaterial* material = col3dmgr->GetDefaultPxMaterial();

		physx::PxShape* shape = PhysXInstance::GetInst().createShape(m_pxBoxGeometry, *material);

		ASSERT_DEBUG(shape, "Shape 생성 실패.");

		shape->setLocalPose(m_offsetPosition);


		uint32 layer = GetOwner()->GetLayer();

		physx::PxFilterData mask = col3dmgr->GetCollisionFilterData(layer);
		shape->setSimulationFilterData(mask);
		
		mask = col3dmgr->GetRaycastFilterData(layer);
		shape->setQueryFilterData(mask);

		AttachShape(shape);
	}
}
