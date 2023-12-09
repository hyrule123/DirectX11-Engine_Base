#include "PCH_Engine.h"
#include "ComMgr.h"

#include "AtExit.h"

#include "iComponent.h"

namespace ehw
{
	std::unordered_map<std::string_view, std::function<std::shared_ptr<iComponent>()>> ComMgr::mUmapComConstructor{};
	std::vector<std::string_view> ComMgr::mComNamesByID{};

	std::shared_ptr<iComponent> ComMgr::GetNewCom(const std::string_view _strKey)
	{
		std::shared_ptr<iComponent> com{};
		const auto& iter = mUmapComConstructor.find(_strKey);
		if (iter != mUmapComConstructor.end())
		{
			com = iter->second();
			com->SetKey(iter->first);
		}
		return com;
	}


	void ComMgr::Init()
	{
		AtExit::AddFunc(ComMgr::Release);
	}
	void ComMgr::Release()
	{
		mUmapComConstructor.clear();
		mComNamesByID.clear();
	}



}

