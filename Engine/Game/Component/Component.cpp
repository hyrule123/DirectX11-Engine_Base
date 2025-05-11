#include "Engine/Game/Component/Component.h"

#include "Engine/Game/GameObject.h"
#include "Engine/Game/Scene.h"

#ifdef max
#undef max
#endif

namespace core {
	Component::Component(const std::string_view key, eComponentOrder _type)
		: Entity(key)
		, m_ComCategory(_type)
		, m_ownerGameObject()
		, m_state(eState::NotInitialzed)
		, m_isStarted(false)
		, m_isEnabled(true)
	{
	}

	Component::Component(const Component& _other)
		: Entity(_other)
		, m_ComCategory(_other.m_ComCategory)
		, m_ownerGameObject()
		, m_state(_other.m_state)
		, m_isStarted(_other.m_isStarted)
		, m_isEnabled(_other.m_isEnabled)
	{
	}

	Component::~Component()
	{
	}

	void Component::SetEnable(bool _bEnable)
	{
		//제거 예약이 되어있거나 동일한 상태일 경우에는 return
		if (m_isEnabled == _bEnable)
		{
			return;
		}

		m_isEnabled = _bEnable;

		auto onEnableDisable = [this]()
			{
				if (m_isEnabled)
				{
					OnEnable();
				}
				else
				{
					OnDisable();
				}
			};

		Scene* scene = m_ownerGameObject->scene();
		if (scene->IsAwaken())
		{
			scene->AddFrameEndJob(onEnableDisable);
		}
		else
		{
			onEnableDisable();
		}
	}
}
