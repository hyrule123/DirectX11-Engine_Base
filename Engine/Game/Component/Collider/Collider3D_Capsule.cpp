#include "Collider3D_Capsule.h"

#include <Engine/Game/Collision/PhysXInstance.h>
#include <Engine/Game/Collision/Collision3D.h>
#include <Engine/Game/GameObject.h>
#include <Engine/Game/Scene.h>
#include <Engine/Game/Component/Transform.h>

namespace core
{
	Collider3D_Capsule::Collider3D_Capsule()
		: Collider3D_Shapes(Collider3D_Capsule::s_static_type_name, eCollider3D_Shape::Capsule)
		, m_offset_position()
		, m_offset_ratio_radius(1.f)
		, m_offset_ratio_half_height(1.f)
	{
	}
	Collider3D_Capsule::~Collider3D_Capsule()
	{
	}
	void Collider3D_Capsule::init()
	{
		Collider3D_Shapes::init();

		Collision3D* col3dMgr = get_collision_manager_3D();
		if (nullptr == col3dMgr)
		{
			ASSERT_DEBUG(nullptr, "Collision3D Manager이 존재하지 않음.");
			return;
		}

		physx::PxMaterial* mtrl = col3dMgr->GetDefaultPxMaterial();
		ASSERT_DEBUG(mtrl, "Default PxMaterial 인스턴스가 없습니다.");

		s_ptr<Transform> tr = get_transform();

		const MATRIX& worldMat = tr->get_world_matrix();
		m_capsule_geometry.radius = worldMat.Axis(eAxis3D::X).Length() * m_offset_ratio_radius;
		m_capsule_geometry.halfHeight = worldMat.Axis(eAxis3D::Y).Length() * m_offset_ratio_half_height;

		physx::PxShape* shape = PhysXInstance::get_inst().GetPhysX().createShape(m_capsule_geometry, *mtrl);

		ASSERT_DEBUG(shape, "PxShape 인스턴스 생성 실패.");

		shape->setLocalPose(m_offset_position);

		ASSERT(false, "미구현");

		//AttachShape(shape);
	}
}
