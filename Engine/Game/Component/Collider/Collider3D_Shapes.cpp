#include "Collider3D_Shapes.h"


#include <Engine/Game/Collision/PhysXInstance.h>

#include <Engine/Game/Collision/CollisionSystem.h>
#include <Engine/Game/Collision/Collision3D.h>
#include <Engine/Game/Component/Transform.h>

#include <Engine/Game/Component/Rigidbody/Com_Rigidbody_Static.h>

#include <Engine/Game/Scene.h>
#include <Engine/Game/GameObject.h>

namespace core
{
	Collider3D_Shapes::Collider3D_Shapes()
		: Collider3D(Collider3D_Shapes::s_static_type_name, eCollider3DType::Rigid)
		, m_px_shapes{}
	{
	}
	Collider3D_Shapes::Collider3D_Shapes(const std::string_view key, eCollider3D_Shape _shape)
		: Collider3D(key, eCollider3DType::Rigid)
		, m_px_shapes{}
	{
	}
	Collider3D_Shapes::~Collider3D_Shapes()
	{
	}

	void Collider3D_Shapes::awake()
	{
		Collider3D::awake();

		s_ptr<GameObject> owner = get_owner();
		ASSERT_DEBUG(owner, "owner GameObject가 없습니다.");

		s_ptr<Rigidbody> rigidbody = owner->get_component<Rigidbody>();

		if (nullptr == rigidbody)
		{
			rigidbody = owner->add_component<Com_Rigidbody_Static>();
			ASSERT_DEBUG(rigidbody, "rigidbody 생성되지 않음.");
		}

		Collision3D* col3d = get_collision_manager_3D();
		
		uint32 layer = owner->get_layer();
		if (col3d)
		{
			m_collision_mask = col3d->GetCollisionFilterData(layer);
			m_raycast_mask = col3d->GetRaycastFilterData(layer);
		}

		std::vector<physx::PxShape*> childShapes = add_shape_from_child();
		for (size_t i = 0; i < childShapes.size(); ++i)
		{
			add_shape(childShapes[i]);
		}
		
		for (size_t i = 0; i < m_px_shapes.size(); ++i)
		{
			bool result = rigidbody->attach_collider_shape(m_px_shapes[i]);
			ASSERT_DEBUG(result, "Shape Attach 실패");
		}
	}

	void Collider3D_Shapes::on_destroy()
	{
		Collider3D::on_destroy();

		for (size_t i = 0; i < m_px_shapes.size(); ++i)
		{
			m_px_shapes[i]->release();
		}
		m_px_shapes.clear();
	}

	void Collider3D_Shapes::on_layer_change(uint32 _layer)
	{
		Collision3D* col3d = get_collision_manager_3D();

		if(col3d)
		{
			//레이어 번호 변경되었을 시 Shape의 Filter Data를 전부 변경한다.
			const physx::PxFilterData collMask = col3d->GetCollisionFilterData(_layer);
			const physx::PxFilterData rayMask = col3d->GetRaycastFilterData(_layer);

			bool changed = (m_collision_mask != collMask || m_raycast_mask != rayMask);

			if (changed)
			{
				for (size_t i = 0; i < m_px_shapes.size(); ++i)
				{
					physx::PxRigidActor* rigidActor = m_px_shapes[i]->getActor();
					if (rigidActor)
					{
						rigidActor->detachShape(*(m_px_shapes[i]));
					}
					m_px_shapes[i]->setSimulationFilterData(m_collision_mask);
					m_px_shapes[i]->setQueryFilterData(m_raycast_mask);
					if (rigidActor)
					{
						rigidActor->attachShape(*(m_px_shapes[i]));
					}
				}
			}
		}
	}




	void Collider3D_Shapes::set_trigger(bool _isTrigger)
	{
		for (size_t i = 0; i < m_px_shapes.size(); ++i)
		{
			physx::PxShapeFlags flags = m_px_shapes[i]->getFlags();

			if (m_is_trigger)
			{
				flags &= ~physx::PxShapeFlag::eSIMULATION_SHAPE;
				flags |= physx::PxShapeFlag::eTRIGGER_SHAPE;
			}
			else
			{
				flags &= ~physx::PxShapeFlag::eTRIGGER_SHAPE;
				flags |= physx::PxShapeFlag::eSIMULATION_SHAPE;
			}

			m_px_shapes[i]->setFlags(flags);
		}
	}

	void Collider3D_Shapes::add_shape(physx::PxShape* _shape)
	{
		if (_shape)
		{
			_shape->userData = this;
			_shape->setSimulationFilterData(m_collision_mask);
			_shape->setQueryFilterData(m_raycast_mask);
			m_px_shapes.push_back(_shape);
		}
	}

	void Collider3D_Shapes::remove_shape(physx::PxShape* _shape)
	{
		if (_shape)
		{
			for (auto iter = m_px_shapes.begin(); iter != m_px_shapes.end(); ++iter)
			{
				if((*iter) == _shape)
				{
					physx::PxRigidActor* actor = (*iter)->getActor();
					if (actor)
					{
						actor->detachShape(**iter);
					}
					
					(*iter)->release();
					m_px_shapes.erase(iter);
					break;
				}
			}
		}
	}

}
