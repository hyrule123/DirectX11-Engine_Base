#include "Engine/Game/Component/Collider/Com_Collider3D_Sphere.h"

#include "Engine/Game/Collision/PhysXInstance.h"
#include "Engine/Game/Collision/Collision3D.h"
#include "Engine/Game/GameObject.h"
#include "Engine/Game/iScene.h"

namespace ehw
{
	Com_Collider3D_Sphere::Com_Collider3D_Sphere()
		: Com_Collider3D_Rigid(eCollider3D_Shape::Sphere)
		, m_offsetRatio_Radius(1.f)
	{
	}
	Com_Collider3D_Sphere::~Com_Collider3D_Sphere()
	{
	}

	void Com_Collider3D_Sphere::Init()
	{
		Com_Collider3D_Rigid::Init();

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

		PhysXSharedPtr<physx::PxShape> shape = PhysXInstance::GetInst().createShape(m_sphereGeometry, *mtrl);

		ASSERT_DEBUG(shape.Get(), "PxShape 인스턴스 생성 실패.");

		shape->setLocalPose(m_offsetPosition);

		AttachShape(shape);

		
	}

}