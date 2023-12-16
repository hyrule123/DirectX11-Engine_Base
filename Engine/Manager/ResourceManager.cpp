
#include "ResourceManager.h"



#include "../Util/AtExit.h"




#include "../tResourceManager.h"
#include "../Resource/Texture.h"
#include "../Resource/Texture.h"
#include "../Resource/Shader/ComputeShaders/GPUInitSetting.h"
#include "../Resource/Shader/ComputeShaders/NormalConvertShader.h"

namespace ehw
{
	std::unordered_map<std::string, std::shared_ptr<iResource>, tHashFunc_StringView, std::equal_to<>> ResourceManager::mArrRes[(int)eResourceType::END]{};

	void ResourceManager::Init()
	{
		AtExit::AddFunc(Release);

		tResourceManager<Texture>::Init("");
		tResourceManager<Texture>::Load("DefaultSprite.png");
		ResourceManagers::CleanUnusedResources();

		tResourceManager<iComputeShader>::Init("");

		auto particle = tResourceManager<iComputeShader>::Load<GPUInitSetting>("GPUInitSetting");

		tResourceManager<iComputeShader>::Load<NormalConvertShader>("NormalConvertShader");

		tResourceManager<iComputeShader>::CleanUnusedResources();
	}
	
	void ResourceManager::Release()
	{
		for (int i = 0; i < (int)eResourceType::END; ++i)
		{
			mArrRes[i].clear();
		}
	}
}
