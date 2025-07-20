
#include "Component.h"

#include <Engine/Game/GameObject.h>
#include <Engine/Game/Scene.h>

#ifdef max
#undef max
#endif

namespace core {
	Component::Component(const std::string_view key, eComponentOrder _type)
		: Entity(key)
		, m_component_order(_type)
		, m_ownerGameObject()
		, m_state(eState::NotInitialzed)
		, m_b_start(false)
		, m_b_enable(true)
	{
	}

	Component::Component(const Component& _other)
		: Entity(_other)
		, m_component_order(_other.m_component_order)
		, m_ownerGameObject()
		, m_state(_other.m_state)
		, m_b_start(_other.m_b_start)
		, m_b_enable(_other.m_b_enable)
	{
	}

	Component::~Component()
	{
	}

	void Component::init()
	{
		Super::init();
	}

	void Component::set_enable(bool _bEnable)
	{
		//제거 예약이 되어있거나 동일한 상태일 경우에는 return
		if (m_b_enable == _bEnable)
		{
			return;
		}

		m_b_enable = _bEnable;

		auto onEnableDisable = [this]()
			{
				if (m_b_enable)
				{
					on_enable();
				}
				else
				{
					on_disable();
				}
			};

		if (m_ownerGameObject.lock())
		{
			s_ptr<Scene> scene = m_ownerGameObject.lock()->get_scene();
			if (scene->is_awaken())
			{
				scene->add_frame_end_job(onEnableDisable);
			}
			else
			{
				onEnableDisable();
			}
		}

	}

	bool Component::update_destroy_state()
	{
		if (eState::destroy == m_state)
		{
			return true;
		}
		else if (eState::DestroyReserved == m_state)
		{
			m_state = eState::destroy;
		}

		return false;
	}
}
