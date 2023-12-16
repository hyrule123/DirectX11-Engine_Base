#include "PCH_Engine.h"
#include "ResourceManagers.h"

#include "Util/AtExit.h"

namespace ehw
{
	std::vector<std::function<void()>> ResourceManagers::m_CleanUnusedResourcesFunction{};

	void ResourceManagers::CleanUnusedResources()
	{
		for (size_t i = 0; i < m_CleanUnusedResourcesFunction.size(); ++i)
		{
			m_CleanUnusedResourcesFunction[i]();
		}
	}

	void ResourceManagers::Init()
	{
		AtExit::AddFunc(Release);
	}

	void ResourceManagers::Release()
	{
		m_CleanUnusedResourcesFunction.clear();
	}

}


