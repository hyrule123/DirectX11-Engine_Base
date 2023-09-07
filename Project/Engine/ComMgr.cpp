#include "PCH_Engine.h"
#include "ComMgr.h"

#include "AtExit.h"

#include "IComponent.h"

namespace mh
{
	std::unordered_map<std::string_view, std::function<IComponent* ()>> ComMgr::mUmapComConstructor{};
	std::vector<std::string_view> ComMgr::mComNamesByID{};

	IComponent* ComMgr::GetNewCom(const std::string_view _strKey)
	{
		IComponent* com = nullptr;
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

