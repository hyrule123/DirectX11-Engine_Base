#include "PCH_Engine.h"
#include "ResourceMgr.h"

#include "AtExit.h"

namespace ehw
{
	std::unordered_map<std::string, std::shared_ptr<iResource>, tHashFunc_StringView, std::equal_to<>> ResourceMgr::mArrRes[(int)eResourceType::END]{};

	void ResourceMgr::Init()
	{
		AtExit::AddFunc(Release);
	}

	void ResourceMgr::Release()
	{
		for (int i = 0; i < (int)eResourceType::END; ++i)
		{
			mArrRes[i].clear();
		}
	}
}
