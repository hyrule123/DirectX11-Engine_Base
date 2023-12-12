#include "../PCH_Engine.h"
#include "ComponentManager.h"



#include "../Util/AtExit.h"

#include "../Game/Component/iComponent.h"

namespace ehw
{
	std::unordered_map<std::string_view, std::function<std::shared_ptr<iComponent>()>> ComponentManager::mUmapComConstructor{};
	std::vector<std::string_view> ComponentManager::mComNamesByID{};

	std::shared_ptr<iComponent> ComponentManager::GetNewComponent(const std::string_view _strKey)
	{
		std::shared_ptr<iComponent> com{};
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
		AtExit::AddFunc(ComponentManager::Release);
	}
	void ComponentManager::Release()
	{
		mUmapComConstructor.clear();
		mComNamesByID.clear();
	}



}

