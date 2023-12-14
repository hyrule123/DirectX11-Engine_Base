#include "PCH_Engine.h"
#include "ResourceManagers.h"

#include "Util/AtExit.h"

namespace ehw
{
	std::vector<std::function<void()>> ResourceManagers::m_CleanFunctions{};

	void ResourceManagers::CleanResources()
	{
		for (size_t i = 0; i < m_CleanFunctions.size(); ++i)
		{
			m_CleanFunctions[i]();
		}
	}

	void ResourceManagers::Init()
	{
		AtExit::AddFunc(Release);
	}

	void ResourceManagers::Release()
	{
	}

}


