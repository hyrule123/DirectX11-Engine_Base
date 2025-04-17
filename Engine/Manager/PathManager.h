#pragma once
#include "Engine/Resource/define_Resource.h"
#include "Engine/Util/StaticSingleton.h"


#include <filesystem>
namespace std
{
	namespace fs = filesystem;
}

namespace core
{
	class PathManager : public StaticSingleton<PathManager>
	{
		friend class StaticSingleton<PathManager>;
		friend class GameEngine;
	public:
		const std::fs::path& GetResPathAbsolute() { return m_absoluteResourceDir; }
		const std::fs::path& GetResPathRelative() { return m_relativeResourceDir; }

		const std::fs::path& GetShaderCSOPath() { return m_relativeDir_ShaderCSO; }

		std::fs::path MakePathStrKey(const std::fs::path& _fullPath);

	private:
		PathManager();
		~PathManager();

	private:
		std::filesystem::path m_absoluteResourceDir;
		std::filesystem::path m_relativeResourceDir;
		std::filesystem::path m_relativeDir_ShaderCSO;
	};


}

