#include "ComponentManager.h"

#include "Engine/Util/AtExit.h"

#include "Engine/Scene/Component/Component.h"

namespace ehw
{
	ComponentManager::ComponentManager()
		: m_comNamesToID{}
		, m_comIDtoInfo{}
	{
	}

	ComponentManager::~ComponentManager()
	{
	}

	void ComponentManager::Init()
	{
		AtExit::AddFunc(std::bind(&ComponentManager::Release, this));
	}
	void ComponentManager::Release()
	{
		m_comNamesToID.clear();
		m_comIDtoInfo.clear();
	}

	std::unique_ptr<iComponent> ComponentManager::GetNewComponent(const std::string_view _strKey)
	{
		std::unique_ptr<iComponent> com{};
		const auto& iter = m_comNamesToID.find(_strKey);
		if (iter != m_comNamesToID.end())
		{
			com = m_comIDtoInfo[iter->second].ctor();
			com->SetStrKey(iter->first);
		}
		return com;
	}

}

