#pragma once
#include "Engine/Resource/define_Resource.h"
#include "Engine/Util/Singleton.h"


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
		PathManager();
		~PathManager();

	public:
		const std::fs::path& GetResPathAbsolute() { return m_absoluteResourceDir; }
		const std::fs::path& GetResPathRelative() { return m_relativeResourceDir; }

		const std::fs::path& GetShaderCSOPath() { return m_relativeDir_ShaderCSO; }

		std::fs::path MakePathStrKey(const std::fs::path& _fullPath);

	private:
		std::filesystem::path m_absoluteResourceDir;
		std::filesystem::path m_relativeResourceDir;
		std::filesystem::path m_relativeDir_ShaderCSO;
	};


}

