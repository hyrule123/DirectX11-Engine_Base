#include "Collider2D_AABB.h"

#include <Engine/Game/GameObject.h>

#include <Engine/Game/Component/Transform.h>

namespace core
{
	Collider2D_AABB::Collider2D_AABB()
		: Collider2D(Collider2D_AABB::s_concrete_class_name, eCollider2D_Shape::AABB)
		, m_offset_scale(100.f)
		, m_left_bottom(0.f, 0.f)
		, m_right_top(100.f, 100.f)
		, m_b_sync_scale_to_transform(false)
	{
	}
	Collider2D_AABB::~Collider2D_AABB()
	{
	}

	void Collider2D_AABB::update_shape()
	{
		s_ptr<Transform> tr = get_owner()->get_component<Transform>();

		if (false == m_b_offset_scale_updated && false == tr->is_transform_updated())
		{
			return;
		}

		const MATRIX& worldMat = tr->get_world_matrix();
		float2 pos = float2(worldMat._41, worldMat._42);
		float2 halfExtentXY = m_offset_scale;

		if (m_b_sync_scale_to_transform)
		{
			halfExtentXY.x *= worldMat.Axis(eAxis3D::X).Length();
			halfExtentXY.y *= worldMat.Axis(eAxis3D::Y).Length();
		}
		halfExtentXY *= 0.5f;

		m_left_bottom.x = pos.x - halfExtentXY.x;
		m_left_bottom.y = pos.y - halfExtentXY.y;
		m_right_top.x = pos.x + halfExtentXY.x;
		m_right_top.y = pos.y + halfExtentXY.y;
	}

	void Collider2D_AABB::frame_end()
	{
		Collider2D::frame_end();
		m_b_offset_scale_updated = false;
	}

	MATRIX Collider2D_AABB::get_collider_matrix()
	{
		MATRIX mat{};
		
		const MATRIX& worldMat = get_transform()->get_world_matrix();

		mat._41 = worldMat._41;
		mat._42 = worldMat._42;

		mat._11 = m_offset_scale.x;
		mat._22 = m_offset_scale.y;

		if (m_b_sync_scale_to_transform)
		{
			mat._11 *= worldMat.Axis(eAxis3D::X).Length();
			mat._22 *= worldMat.Axis(eAxis3D::Y).Length();
		}

		return mat;
	}
}
