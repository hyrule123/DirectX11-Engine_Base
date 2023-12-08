#include "PCH_Engine.h"

#include "iComponent.h"

#include "GameObject.h"
#include "iScene.h"

#include "json-cpp\jsonSaveLoad.h"

namespace ehw
{
	iComponent::iComponent(eComponentCategory _type)
		: m_ComCategory(_type)
		, m_ComTypeID()
		, m_Owner(nullptr)
		, m_State(eState::Enabled)
	{
	}

	iComponent::iComponent(const iComponent& _other)
		: Entity(_other)
		, m_ComCategory(_other.m_ComCategory)
		, m_ComTypeID()
		, m_Owner(nullptr)
		, m_State(_other.m_State)
	{
	}

	iComponent::~iComponent()
	{
	}
	eResult iComponent::SaveJson(Json::Value* _pJson)
	{
		if (nullptr == _pJson)
		{
			return eResult::Fail_Nullptr;
		}
		eResult result = Entity::SaveJson(_pJson);
		if (eResultFail(result))
		{
			return result;
		}

		Json::SaveLoad::SaveValue(_pJson, JSON_KEY_PAIR(m_ComCategory));

		return eResult::Success;
	}
	eResult iComponent::LoadJson(const Json::Value* _pJson)
	{
		if (nullptr == _pJson)
		{
			return eResult::Fail_Nullptr;
		}
		eResult result = Entity::LoadJson(_pJson);
		if (eResultFail(result))
		{
			return result;
		}

		return eResult::Success;
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
					m_State = eState::Enabled;
					OnEnable();
				}
				else
				{
					m_State = eState::Disabled;
					OnDisable();
				}
			};

		if (m_OwnerScene->IsAwaken())
		{
			m_OwnerScene->AddFrameEndJob(onEnableDisable);
		}
		else
		{
			onEnableDisable();
		}

	}

	void iComponent::Destroy()
	{
		if (IsDestroyed())
			return;

		auto DestroyFunc = [this]()
			{
				m_State = eState::Destroy;
			};

		if (m_OwnerScene->IsAwaken())
		{
			m_OwnerScene->AddFrameEndJob(DestroyFunc);
		}
		else
		{
			DestroyFunc();
		}
	}
}
