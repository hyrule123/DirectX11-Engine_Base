#include "Engine/Game/Component/Collider/Com_Collider3D_Cube.h"

#include "Engine/Game/Component/Transform.h"

#include "Engine/Game/Collision/Collision3D.h"

#include "Engine/Game/Collision/PhysXInstance.h"

#include "Engine/Game/GameObject.h"
#include "Engine/Game/Scene.h"

namespace ehw
{
	Com_Collider3D_Cube::Com_Collider3D_Cube()
		: Com_Collider3D_Shapes(Com_Collider3D_Cube::concrete_class_name, eCollider3D_Shape::Cube)
		, m_offsetHalfScale(1.f)
		, m_offsetPosition(physx::PxIdentity)
		, m_offsetScale(1.f)
	{
	}
	Com_Collider3D_Cube::~Com_Collider3D_Cube()
	{
	}

	std::vector<physx::PxShape*> Com_Collider3D_Cube::AddShapeFromChild()
	{
		std::vector<physx::PxShape*> ret{};

		Transform* tr = GetTransform();

		m_offsetHalfScale.halfExtents = (tr->get_world_scale() * m_offsetScale) * 0.5f;

		Collision3D* col3dmgr = GetCollision3D();
		if (nullptr == col3dmgr)
		{
			ASSERT_DEBUG(nullptr, "Collision3D Manager이 존재하지 않음.");
			return ret;
		}

		physx::PxMaterial* material = col3dmgr->GetDefaultPxMaterial();

		physx::PxShape* shape = PhysXInstance::GetInst().GetPhysX().createShape(m_offsetHalfScale, *material, true);

		ASSERT_DEBUG(shape, "Shape 생성 실패.");

		shape->setLocalPose(m_offsetPosition);

		ret.push_back(shape);

		return ret;
	}
}
