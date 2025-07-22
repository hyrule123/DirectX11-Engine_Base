#pragma once
#include <Engine/Game/Component/Collider/Collider.h>
#include <Engine/define_Enum.h>

#include <Engine/Game/Collision/PhysX.h>

namespace core
{
	class Transform;
	class Collision3D;
	class Collider3D : public Collider
	{
		CLASS_INFO(Collider3D, Collider);
		CLONE_DISABLE(Collider3D);
	public:
		Collider3D(eCollider3DType _col3dType);
		
		virtual ~Collider3D();

		eResult serialize_json(JsonSerializer* _ser) const override { return eResult(); };
		eResult deserialize_json(const JsonSerializer* _ser) override { return eResult(); };

		void init() override;
		void awake() override;

		MATRIX get_collider_matrix() override { return MATRIX{}; };

		//Start 호출 되기 전에만 동작함.
		void set_sync_scale_to_transform(bool enable) { m_b_sync_scale_to_transform = enable; }

	protected:
		inline bool is_sync_scale_to_transform() const { return m_b_sync_scale_to_transform; }
		inline Collision3D* get_collision_manager_3D() { return m_collision_manager_3D; }

	private:
		eCollider3DType				m_collider_type_3D;

		bool						m_b_sync_scale_to_transform;

		Collision3D*				m_collision_manager_3D;
	};
}


