
#include "ResourceManager.h"



#include "../Util/AtExit.h"

namespace ehw
{
	std::unordered_map<std::string, std::shared_ptr<iResource>, tHashFunc_StringView, std::equal_to<>> ResourceManager::mArrRes[(int)eResourceType::END]{};

	void ResourceManager::Init()
	{
		AtExit::AddFunc(Release);
	}

	void ResourceManager::Release()
	{
		for (int i = 0; i < (int)eResourceType::END; ++i)
		{
			mArrRes[i].clear();
		}
	}
}
