#include "Engine/Game/Component/Collider/Com_Collider2D_AABB.h"

#include "Engine/Game/GameObject.h"

namespace ehw
{
	Com_Collider2D_AABB::Com_Collider2D_AABB()
		: iCollider2D(eCollider2D_Shape::AABB)
		, m_transform()
		, m_offsetScale(100.f)
		, m_leftBottom(0.f, 0.f)
		, m_rightTop(100.f, 100.f)
		, m_isSyncScaleToTransform(false)
	{
	}
	Com_Collider2D_AABB::~Com_Collider2D_AABB()
	{
	}

	void Com_Collider2D_AABB::Awake()
	{
		m_transform = GetOwner()->Transform();
	}

	void Com_Collider2D_AABB::Update()
	{
		const MATRIX& worldMat = m_transform->GetWorldMatrix();
		float2 pos = float2(worldMat._41, worldMat._42);
		float2 halfExtentXY = m_offsetScale;

		if (m_isSyncScaleToTransform)
		{
			halfExtentXY.x *= worldMat.Axis(eAxis4D::X).Length();
			halfExtentXY.y *= worldMat.Axis(eAxis4D::Y).Length();
		}
		halfExtentXY *= 0.5f;

		m_leftBottom.x = pos.x - halfExtentXY.x;
		m_leftBottom.y = pos.y - halfExtentXY.y;
		m_rightTop.x = pos.x + halfExtentXY.x;
		m_rightTop.y = pos.y + halfExtentXY.y;
	}
}
