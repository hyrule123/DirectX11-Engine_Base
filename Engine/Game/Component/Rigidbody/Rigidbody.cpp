
#include "Rigidbody.h"

#include <Engine/Game/GameObject.h>
#include <Engine/Game/Component/Transform.h>
#include <Engine/Game/Scene.h>
#include <Engine/Game/Collision/CollisionSystem.h>
#include <Engine/Game/Collision/Collision3D.h>

namespace core
{
	Rigidbody::Rigidbody()
		: Super(s_component_order)
		, m_rigid_actor{}
		, m_shapes_modified{true}
	{
	}

	Rigidbody::~Rigidbody()
	{
	}

	void Rigidbody::init()
	{
		Super::init();

		m_rigid_actor.Set(create_rigidbody());
		ASSERT_DEBUG(m_rigid_actor, "PxActor 생성되지 않음.");
		
		if (m_rigid_actor)
		{
			m_rigid_actor->userData = this;
		}
	}
	void Rigidbody::on_enable()
	{
		s_ptr<GameObject> owner = get_owner();
		ASSERT(owner, "owner 게임오브젝트가 없습니다.");
		s_ptr<Scene> scene = owner->get_scene();
		ASSERT(scene, "scene이 없습니다.");

		CollisionSystem* colsys = scene->get_collision_system();
		if (colsys)
		{
			Collision3D* col3d = colsys->get_collision_manager_3D();
			if (col3d)
			{
				bool ret = col3d->AddPxActor(m_rigid_actor.Get());
				ASSERT_DEBUG(ret, "AddPxActor 실패.");
			}
		}
	}
	void Rigidbody::on_disable()
	{
		physx::PxScene* scene = m_rigid_actor->getScene();
		if (scene)
		{
			scene->removeActor(*(static_cast<physx::PxActor*>(m_rigid_actor.Get())));
		}
	}

	void Rigidbody::on_destroy()
	{
		m_rigid_actor.Reset();
	}


	bool Rigidbody::attach_collider_shape(physx::PxShape* _pxShape)
	{
		bool ret = false;
		if (m_rigid_actor)
		{
			ret = m_rigid_actor->attachShape(*_pxShape);
			m_shapes_modified = true;
		}
		return ret;
	}
	void Rigidbody::detach_collider_shape(physx::PxShape* _pxShape)
	{
		if (m_rigid_actor)
		{
			m_rigid_actor->detachShape(*_pxShape);
			m_shapes_modified = true;
		}
	}
	void Rigidbody::sync_to_physX_global_pose()
	{
		s_ptr<GameObject> owner = get_owner();
		
		if (nullptr == owner) { 
			return; 
		}

		s_ptr<Transform> tr = owner->get_component<Transform>();
		if (false == tr->is_transform_updated())
		{
			return;
		}

		physx::PxTransformT<float> transform{};
		transform.p = tr->get_world_position();
		transform.q = tr->get_local_rotation();

		m_rigid_actor->setGlobalPose(transform);
	}

	void Rigidbody::fetch_from_physX_global_pose(const physx::PxTransform& _pxTransform)
	{
		s_ptr<GameObject> owner = get_owner();

		if (nullptr == owner) { return; }
		s_ptr<Transform> tr = owner->get_component<Transform>();

		tr->set_world_position(_pxTransform.p);
		tr->set_local_rotation(_pxTransform.q);
	}


	void Rigidbody::set_gravity(bool enable)
	{
		if (m_rigid_actor)
		{
			auto flags = m_rigid_actor->getActorFlags();
			if (enable)
			{
				flags &= ~physx::PxActorFlag::eDISABLE_GRAVITY;
			}
			else
			{
				flags |= physx::PxActorFlag::eDISABLE_GRAVITY;
			}

			m_rigid_actor->setActorFlags(flags);
		}
	}

	bool Rigidbody::is_gravity_enabled() const
	{
		if (m_rigid_actor)
		{
			auto flags = m_rigid_actor->getActorFlags();

			if (!(physx::PxActorFlag::eDISABLE_GRAVITY & flags))
			{
				return true;
			}
		}

		return false;
	}
}
