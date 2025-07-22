#include "Collider3D_Cube.h"

#include <Engine/Game/Component/Transform.h>

#include <Engine/Game/Collision/Collision3D.h>

#include <Engine/Game/Collision/PhysXInstance.h>

#include <Engine/Game/GameObject.h>
#include <Engine/Game/Scene.h>

namespace core
{
	Collider3D_Cube::Collider3D_Cube()
		: Collider3D_Shapes(eCollider3D_Shape::Cube)
		, m_offset_half_scale(1.f)
		, m_offset_position(physx::PxIdentity)
		, m_offset_scale(1.f)
	{
	}
	Collider3D_Cube::~Collider3D_Cube()
	{
	}

	std::vector<physx::PxShape*> Collider3D_Cube::add_shape_from_child()
	{
		std::vector<physx::PxShape*> ret{};

		s_ptr<Transform> tr = get_transform();

		m_offset_half_scale.halfExtents = (tr->get_world_scale() * m_offset_scale) * 0.5f;

		Collision3D* col3dmgr = get_collision_manager_3D();
		if (nullptr == col3dmgr)
		{
			ASSERT_DEBUG(nullptr, "Collision3D Manager이 존재하지 않음.");
			return ret;
		}

		physx::PxMaterial* material = col3dmgr->GetDefaultPxMaterial();

		physx::PxShape* shape = PhysXInstance::get_inst().GetPhysX().createShape(m_offset_half_scale, *material, true);

		ASSERT_DEBUG(shape, "Shape 생성 실패.");

		shape->setLocalPose(m_offset_position);

		ret.push_back(shape);

		return ret;
	}
}
