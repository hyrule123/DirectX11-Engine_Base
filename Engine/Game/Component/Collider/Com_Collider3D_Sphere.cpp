#include "Engine/Game/Component/Collider/Com_Collider3D_Sphere.h"

#include "Engine/Game/Collision/PhysXInstance.h"
#include "Engine/Game/Collision/Collision3D.h"
#include "Engine/Game/GameObject.h"
#include "Engine/Game/Scene.h"
#include "Engine/Game/Component/Transform.h"

namespace core
{
	Com_Collider3D_Sphere::Com_Collider3D_Sphere()
		: Com_Collider3D_Shapes(Com_Collider3D_Sphere::s_static_type_name, eCollider3D_Shape::Sphere)
		, m_offsetRatio_Radius(1.f)
	{
	}
	Com_Collider3D_Sphere::~Com_Collider3D_Sphere()
	{
	}

	void Com_Collider3D_Sphere::init()
	{
		Com_Collider3D_Shapes::init();

		Collision3D* col3dMgr = GetCollision3D();
		if (nullptr == col3dMgr)
		{
			ASSERT_DEBUG(nullptr, "Collision3D Manager이 존재하지 않음.");
			return;
		}
		physx::PxMaterial* mtrl = col3dMgr->GetDefaultPxMaterial();

		s_ptr<Transform> tr = GetTransform();

		const MATRIX& worldMat = tr->get_world_matrix();
		m_sphereGeometry.radius = worldMat.Axis(eAxis3D::X).Length() * m_offsetRatio_Radius;

		physx::PxShape* shape = PhysXInstance::get_inst().GetPhysX().createShape(m_sphereGeometry, *mtrl);

		ASSERT_DEBUG(shape, "PxShape 인스턴스 생성 실패.");

		shape->setLocalPose(m_offsetPosition);
		ASSERT(false, "미구현");
		//AttachShape(shape);
	}

}
