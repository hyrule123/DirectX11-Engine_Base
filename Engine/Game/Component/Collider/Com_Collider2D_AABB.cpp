#include "Engine/Game/Component/Collider/Com_Collider2D_AABB.h"

#include "Engine/Game/GameObject.h"

#include "Engine/Game/Component/Transform.h"

namespace core
{
	Com_Collider2D_AABB::Com_Collider2D_AABB()
		: Collider2D(Com_Collider2D_AABB::concrete_class_name, eCollider2D_Shape::AABB)
		, m_offsetScale(100.f)
		, m_leftBottom(0.f, 0.f)
		, m_rightTop(100.f, 100.f)
		, m_isSyncScaleToTransform(false)
	{
	}
	Com_Collider2D_AABB::~Com_Collider2D_AABB()
	{
	}

	void Com_Collider2D_AABB::UpdateShape()
	{
		Transform* tr = gameObject()->GetComponent<Transform>();

		if (false == m_isOffsetScaleUpdated && false == tr->is_transform_updated())
		{
			return;
		}

		const MATRIX& worldMat = tr->get_world_matrix();
		float2 pos = float2(worldMat._41, worldMat._42);
		float2 halfExtentXY = m_offsetScale;

		if (m_isSyncScaleToTransform)
		{
			halfExtentXY.x *= worldMat.Axis(eAxis3D::X).Length();
			halfExtentXY.y *= worldMat.Axis(eAxis3D::Y).Length();
		}
		halfExtentXY *= 0.5f;

		m_leftBottom.x = pos.x - halfExtentXY.x;
		m_leftBottom.y = pos.y - halfExtentXY.y;
		m_rightTop.x = pos.x + halfExtentXY.x;
		m_rightTop.y = pos.y + halfExtentXY.y;
	}

	void Com_Collider2D_AABB::frame_end()
	{
		Collider2D::frame_end();
		m_isOffsetScaleUpdated = false;
	}

	MATRIX Com_Collider2D_AABB::GetColliderMatrix()
	{
		MATRIX mat{};
		
		const MATRIX& worldMat = GetTransform()->get_world_matrix();

		mat._41 = worldMat._41;
		mat._42 = worldMat._42;

		mat._11 = m_offsetScale.x;
		mat._22 = m_offsetScale.y;

		if (m_isSyncScaleToTransform)
		{
			mat._11 *= worldMat.Axis(eAxis3D::X).Length();
			mat._22 *= worldMat.Axis(eAxis3D::Y).Length();
		}

		return mat;
	}
}
