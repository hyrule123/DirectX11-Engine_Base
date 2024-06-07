#include "ComponentManager.h"

#include "Engine/Util/AtExit.h"

#include "Engine/Scene/Component/iComponent.h"

namespace ehw
{
	ComponentManager::ComponentManager()
		: mUmapComConstructor{}
		, mComNamesByID{}
	{
	}

	ComponentManager::~ComponentManager()
	{
	}

	std::unique_ptr<iComponent> ComponentManager::GetNewComponent(const std::string_view _strKey)
	{
		std::unique_ptr<iComponent> com{};
		const auto& iter = mUmapComConstructor.find(_strKey);
		if (iter != mUmapComConstructor.end())
		{
			com = iter->second();
			com->SetStrKey(iter->first);
		}
		return com;
	}

	void ComponentManager::Init()
	{
		AtExit::AddFunc(std::bind(&ComponentManager::Release, this));
	}
	void ComponentManager::Release()
	{
		mUmapComConstructor.clear();
		mComNamesByID.clear();
	}



}

