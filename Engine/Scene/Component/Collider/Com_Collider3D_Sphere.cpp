#include "Engine/Scene/Component/Collider/Com_Collider3D_Sphere.h"

#include "Engine/Scene/Collision/PhysXInstance.h"
#include "Engine/Scene/Collision/Collision3D.h"
#include "Engine/Scene/GameObject.h"
#include "Engine/Scene/iScene.h"

namespace ehw
{
	Com_Collider3D_Sphere::Com_Collider3D_Sphere()
		: Com_Collider3D_Shapes(eCollider3D_Shape::Sphere)
		, m_offsetRatio_Radius(1.f)
	{
	}
	Com_Collider3D_Sphere::~Com_Collider3D_Sphere()
	{
	}

	void Com_Collider3D_Sphere::Init()
	{
		Com_Collider3D_Shapes::Init();

		Collision3D* col3dMgr = GetCollision3D();
		if (nullptr == col3dMgr)
		{
			ASSERT_DEBUG(nullptr, "Collision3D Manager이 존재하지 않음.");
			return;
		}
		physx::PxMaterial* mtrl = col3dMgr->GetDefaultPxMaterial();

		Com_Transform* tr = GetTransform();

		const MATRIX& worldMat = tr->GetWorldMatrix();
		m_sphereGeometry.radius = worldMat.Axis(eAxis3D::X).Length() * m_offsetRatio_Radius;

		physx::PxShape* shape = PhysXInstance::GetInst().createShape(m_sphereGeometry, *mtrl);

		ASSERT_DEBUG(shape, "PxShape 인스턴스 생성 실패.");

		shape->setLocalPose(m_offsetPosition);
		ASSERT(false, "미구현");
		//AttachShape(shape);

		
	}

}
