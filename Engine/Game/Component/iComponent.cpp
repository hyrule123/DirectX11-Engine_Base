
#include "Engine/Game/Component/iComponent.h"



#include "Engine/Game/GameObject.h"
#include "Engine/Game/iScene.h"

namespace ehw
{
	iComponent::iComponent(eComponentCategory _type)
		: m_ComCategory(_type)
		, m_ComTypeID()
		, m_ownerGameObject()
		, m_state(eState::Enabled)
	{
	}

	iComponent::iComponent(const iComponent& _other)
		: Entity(_other)
		, m_ComCategory(_other.m_ComCategory)
		, m_ComTypeID()
		, m_ownerGameObject()
		, m_state(_other.m_state)
	{
	}

	iComponent::~iComponent()
	{
	}

	void iComponent::SetEnable(bool _bEnable)
	{
		//제거 예약이 되어있거나 동일한 상태일 경우에는 return
		if (IsDestroyed() || IsEnabled() == _bEnable)
		{
			return;
		}

		auto onEnableDisable = [this, _bEnable]()
			{
				if (_bEnable)
				{
					m_state = eState::Enabled;
					OnEnable();
				}
				else
				{
					m_state = eState::Disabled;
					OnDisable();
				}
			};

		iScene* scene = m_ownerGameObject->GetOwnerScene();
		if (scene->IsAwaken())
		{
			scene->AddFrameEndJob(onEnableDisable);
		}
		else
		{
			onEnableDisable();
		}

	}


	bool iComponent::NeedRemove()
	{
		if (eState::Destroy == m_state)
		{
			return true;
		}
		else if (eState::DestroyReserved == m_state)
		{
			m_state = eState::Destroy;
		}

		return false;
	}
}
