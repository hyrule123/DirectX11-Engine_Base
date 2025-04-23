#pragma once
#include "Engine/define_Macro.h"
#include "Engine/Util/Singleton.h"

#include <functional>


namespace core
{
	class ResourceManagers : public Singleton<ResourceManagers>
	{
		friend class Singleton<ResourceManagers>;
		friend class GameEngine;

	public:
		void CleanUnusedResources();

		void AddUnusedResourceCleanFunc(std::function<void()>&& _func) {
			ASSERT_DEBUG(nullptr != _func, "인자로 들어온 함수가 nullptr입니다.");
			m_CleanUnusedResourcesFunction.emplace_back(_func);
		}
		
	private:
		ResourceManagers();
		~ResourceManagers();
		void init_resource_managers();
		void load_default_resources();

		void create_default_buffers();
		void load_default_shaders();
		void load_default_textures();
		void load_default_meshes();
		void load_default_materials();

		std::vector<std::function<void()>> m_CleanUnusedResourcesFunction;
	};
}



