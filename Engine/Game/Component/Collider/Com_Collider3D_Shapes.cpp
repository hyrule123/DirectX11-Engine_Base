#include "Engine/Game/Component/Collider/Com_Collider3D_Shapes.h"


#include "Engine/Game/Collision/PhysXInstance.h"

#include "Engine/Game/Collision/CollisionSystem.h"
#include "Engine/Game/Collision/Collision3D.h"
#include "Engine/Game/Component/Transform.h"

#include "Engine/Game/Component/Rigidbody/Com_Rigidbody_Static.h"

#include "Engine/Game/Scene.h"
#include "Engine/Game/GameObject.h"

namespace ehw
{
	Com_Collider3D_Shapes::Com_Collider3D_Shapes()
		: Collider3D(Com_Collider3D_Shapes::concrete_name, eCollider3DType::Rigid)
		, m_pxShapes{}
	{
	}
	Com_Collider3D_Shapes::Com_Collider3D_Shapes(const std::string_view key, eCollider3D_Shape _shape)
		: Collider3D(key, eCollider3DType::Rigid)
		, m_pxShapes{}
	{
	}
	Com_Collider3D_Shapes::~Com_Collider3D_Shapes()
	{
	}

	void Com_Collider3D_Shapes::Awake()
	{
		Collider3D::Awake();

		Rigidbody* rigidbody = 
			static_cast<Rigidbody*>(gameObject()->GetComponent(eComponentCategory::Rigidbody));

		if (nullptr == rigidbody)
		{
			rigidbody = gameObject()->AddComponent<Com_Rigidbody_Static>();
			ASSERT_DEBUG(rigidbody, "rigidbody 생성되지 않음.");
		}

		Collision3D* col3d = GetCollision3D();
		
		uint32 layer = gameObject()->GetLayer();
		if (col3d)
		{
			m_collisionMask = col3d->GetCollisionFilterData(layer);
			m_raycastMask = col3d->GetRaycastFilterData(layer);
		}

		std::vector<physx::PxShape*> childShapes = AddShapeFromChild();
		for (size_t i = 0; i < childShapes.size(); ++i)
		{
			AddShape(childShapes[i]);
		}
		
		for (size_t i = 0; i < m_pxShapes.size(); ++i)
		{
			bool result = rigidbody->AttachColliderShape(m_pxShapes[i]);
			ASSERT_DEBUG(result, "Shape Attach 실패");
		}
	}

	void Com_Collider3D_Shapes::OnDestroy()
	{
		Collider3D::OnDestroy();

		for (size_t i = 0; i < m_pxShapes.size(); ++i)
		{
			m_pxShapes[i]->release();
		}
		m_pxShapes.clear();
	}

	void Com_Collider3D_Shapes::OnLayerChange(uint32 _layer)
	{
		Collision3D* col3d = GetCollision3D();

		if(col3d)
		{
			//레이어 번호 변경되었을 시 Shape의 Filter Data를 전부 변경한다.
			const physx::PxFilterData collMask = col3d->GetCollisionFilterData(_layer);
			const physx::PxFilterData rayMask = col3d->GetRaycastFilterData(_layer);

			bool changed = (m_collisionMask != collMask || m_raycastMask != rayMask);

			if (changed)
			{
				for (size_t i = 0; i < m_pxShapes.size(); ++i)
				{
					physx::PxRigidActor* rigidActor = m_pxShapes[i]->getActor();
					if (rigidActor)
					{
						rigidActor->detachShape(*(m_pxShapes[i]));
					}
					m_pxShapes[i]->setSimulationFilterData(m_collisionMask);
					m_pxShapes[i]->setQueryFilterData(m_raycastMask);
					if (rigidActor)
					{
						rigidActor->attachShape(*(m_pxShapes[i]));
					}
				}
			}
		}
	}




	void Com_Collider3D_Shapes::SetTrigger(bool _isTrigger)
	{
		for (size_t i = 0; i < m_pxShapes.size(); ++i)
		{
			physx::PxShapeFlags flags = m_pxShapes[i]->getFlags();

			if (m_isTrigger)
			{
				flags &= ~physx::PxShapeFlag::eSIMULATION_SHAPE;
				flags |= physx::PxShapeFlag::eTRIGGER_SHAPE;
			}
			else
			{
				flags &= ~physx::PxShapeFlag::eTRIGGER_SHAPE;
				flags |= physx::PxShapeFlag::eSIMULATION_SHAPE;
			}

			m_pxShapes[i]->setFlags(flags);
		}
	}

	void Com_Collider3D_Shapes::AddShape(physx::PxShape* _shape)
	{
		if (_shape)
		{
			_shape->userData = this;
			_shape->setSimulationFilterData(m_collisionMask);
			_shape->setQueryFilterData(m_raycastMask);
			m_pxShapes.push_back(_shape);
		}
	}

	void Com_Collider3D_Shapes::RemoveShape(physx::PxShape* _shape)
	{
		if (_shape)
		{
			for (auto iter = m_pxShapes.begin(); iter != m_pxShapes.end(); ++iter)
			{
				if((*iter) == _shape)
				{
					physx::PxRigidActor* actor = (*iter)->getActor();
					if (actor)
					{
						actor->detachShape(**iter);
					}
					
					(*iter)->release();
					m_pxShapes.erase(iter);
					break;
				}
			}
		}
	}

}
