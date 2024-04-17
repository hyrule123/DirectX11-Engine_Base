#include "Engine/Game/Component/Collider/Com_Collider3D_Capsule.h"

#include "Engine/Game/Collision/PhysXInstance.h"
#include "Engine/Game/Collision/Collision3D.h"
#include "Engine/Game/GameObject.h"
#include "Engine/Game/iScene.h"

namespace ehw
{
	Com_Collider3D_Capsule::Com_Collider3D_Capsule()
		: Com_Collider3D_Shapes(eCollider3D_Shape::Capsule)
		, m_offsetPosition()
		, m_offsetRatio_Radius(1.f)
		, m_offsetRatio_HalfHeight(1.f)
	{
	}
	Com_Collider3D_Capsule::~Com_Collider3D_Capsule()
	{
	}
	void Com_Collider3D_Capsule::Init()
	{
		Com_Collider3D_Shapes::Init();

		Collision3D* col3dMgr = GetCollision3D();
		if (nullptr == col3dMgr)
		{
			ASSERT_DEBUG(nullptr, "Collision3D Manager이 존재하지 않음.");
			return;
		}

		physx::PxMaterial* mtrl = col3dMgr->GetDefaultPxMaterial();
		ASSERT_DEBUG(mtrl, "Default PxMaterial 인스턴스가 없습니다.");

		Com_Transform* tr = GetTransform();

		const MATRIX& worldMat = tr->GetWorldMatrix();
		m_capsuleGeometry.radius = worldMat.Axis(eAxis3D::X).Length() * m_offsetRatio_Radius;
		m_capsuleGeometry.halfHeight = worldMat.Axis(eAxis3D::Y).Length() * m_offsetRatio_HalfHeight;

		physx::PxShape* shape = PhysXInstance::GetInst().createShape(m_capsuleGeometry, *mtrl);

		ASSERT_DEBUG(shape, "PxShape 인스턴스 생성 실패.");

		shape->setLocalPose(m_offsetPosition);

		ASSERT(false, "미구현");

		//AttachShape(shape);
	}
}
