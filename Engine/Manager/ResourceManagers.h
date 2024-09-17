#pragma once
#include "Engine/define_Macro.h"
#include "Engine/Util/StaticSingleton.h"

#include <functional>


namespace ehw
{
	class ResourceManagers : public StaticSingleton<ResourceManagers>
	{
		friend class StaticSingleton<ResourceManagers>;
		friend class GameEngine;

	public:
		void CleanUnusedResources();

		inline void AddUnusedResourceCleanFunc(std::function<void()>&& _func);
		
	private:
		ResourceManagers();
		~ResourceManagers();
		void init();
		void release();

		void init_static_variables();
		void release_static_variables();

		std::vector<std::function<void()>> m_CleanUnusedResourcesFunction;
	};

	inline void ResourceManagers::AddUnusedResourceCleanFunc(std::function<void()>&& _func)
	{
		ASSERT_DEBUG(nullptr != _func, "인자로 들어온 함수가 nullptr입니다.");
		m_CleanUnusedResourcesFunction.emplace_back(_func);
	}
}



