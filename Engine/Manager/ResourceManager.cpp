
#include "ResourceManager.h"



#include "../Util/AtExit.h"




#include "../tResourceManager.h"
#include "../Resource/Texture.h"

namespace ehw
{
	std::unordered_map<std::string, std::shared_ptr<iResource>, tHashFunc_StringView, std::equal_to<>> ResourceManager::mArrRes[(int)eResourceType::END]{};

	void ResourceManager::Init()
	{
		AtExit::AddFunc(Release);

		tResourceManager<Texture>::Load("DefaultSprite.png");
		ResourceManagers::CleanUnusedResources();
		
	}

	void ResourceManager::Release()
	{
		for (int i = 0; i < (int)eResourceType::END; ++i)
		{
			mArrRes[i].clear();
		}
	}
}
