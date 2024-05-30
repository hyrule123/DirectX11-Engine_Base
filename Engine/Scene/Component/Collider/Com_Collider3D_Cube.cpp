#include "Engine/Scene/Component/Collider/Com_Collider3D_Cube.h"

#include "Engine/Scene/Component/Transform/Com_Transform.h"

#include "Engine/Scene/Collision/Collision3D.h"

#include "Engine/Scene/Collision/PhysXInstance.h"

#include "Engine/Scene/GameObject.h"
#include "Engine/Scene/iScene.h"

namespace ehw
{
	Com_Collider3D_Cube::Com_Collider3D_Cube()
		: Com_Collider3D_Shapes(eCollider3D_Shape::Cube)
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

		Com_Transform* tr = GetTransform();

		m_offsetHalfScale.halfExtents = (tr->GetWorldScale() * m_offsetScale) * 0.5f;

		Collision3D* col3dmgr = GetCollision3D();
		if (nullptr == col3dmgr)
		{
			ASSERT_DEBUG(nullptr, "Collision3D Manager이 존재하지 않음.");
			return ret;
		}

		physx::PxMaterial* material = col3dmgr->GetDefaultPxMaterial();

		physx::PxShape* shape = PhysXInstance::GetInst().createShape(m_offsetHalfScale, *material, true);

		ASSERT_DEBUG(shape, "Shape 생성 실패.");

		shape->setLocalPose(m_offsetPosition);

		ret.push_back(shape);

		return ret;
	}
}
