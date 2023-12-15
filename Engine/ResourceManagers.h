#pragma once
#include "define_Macro.h"

#include <functional>


namespace ehw
{
	class ResourceManagers
	{
		friend class Application;

	public:
		static void CleanUnusedResources();

		static inline void AddUnusedResourceCleanFunc(std::function<void()>&& _func);
		
	private:
		static void Init();
		static void Release();

		

		static std::vector<std::function<void()>> m_CleanUnusedResourcesFunction;
	};

	inline void ResourceManagers::AddUnusedResourceCleanFunc(std::function<void()>&& _func)
	{
		ASSERT_DEBUG(nullptr != _func, "인자로 들어온 함수가 nullptr입니다.");
		m_CleanUnusedResourcesFunction.emplace_back(_func);
	}
}



