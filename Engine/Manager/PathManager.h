#pragma once
#include <Engine/Resource/define_Resource.h>
#include <Engine/Util/Singleton.h>


#include <filesystem>
namespace std
{
	namespace fs = filesystem;
}

namespace core
{
	class PathManager
	{
		friend class GameEngine;

		DECLARE_SINGLETON(PathManager);
	private:
		PathManager();
		void init();

		~PathManager();

	public:
		const std::fs::path& get_absolute_resource_directory() { return m_absolute_resource_directory; }
		const std::fs::path& get_relative_resource_directory() { return m_relative_resource_directory; }

		const std::fs::path& get_relative_shader_directory() { return m_relative_shader_directory; }

		std::fs::path make_path_strkey(const std::fs::path& _fullPath);

	private:
		std::filesystem::path m_absolute_resource_directory;
		std::filesystem::path m_relative_resource_directory;
		std::filesystem::path m_relative_shader_directory;
	};


}

