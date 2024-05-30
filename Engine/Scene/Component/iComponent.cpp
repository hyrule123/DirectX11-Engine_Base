
#include "Engine/Scene/Component/iComponent.h"



#include "Engine/Scene/GameObject.h"
#include "Engine/Scene/iScene.h"

namespace ehw
{
	iComponent::iComponent(eComponentCategory _type)
		: m_ComCategory(_type)
		, m_ComTypeID()
		, m_ownerGameObject()
		, m_state(eState::NotInitialzed)
		, m_isStarted(false)
		, m_isEnabled(true)
	{
	}

	iComponent::iComponent(const iComponent& _other)
		: Entity(_other)
		, m_ComCategory(_other.m_ComCategory)
		, m_ComTypeID()
		, m_ownerGameObject()
		, m_state(_other.m_state)
		, m_isStarted(_other.m_isStarted)
		, m_isEnabled(_other.m_isEnabled)
	{
	}

	iComponent::~iComponent()
	{
	}

	void iComponent::SetEnable(bool _bEnable)
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

		iScene* scene = m_ownerGameObject->GetScene();
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
