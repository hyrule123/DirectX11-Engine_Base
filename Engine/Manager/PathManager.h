#pragma once



#include "../Resource/define_Resource.h"


#include <filesystem>
namespace std
{
	namespace fs = filesystem;
}

namespace ehw
{
	class PathManager
	{
		friend class Application;
	public:
		static void Init();
		
	public:
		static const std::fs::path& GetResPathAbsolute() { return m_absoluteResourceDir; }
		static const std::fs::path& GetResPathRelative() { return m_relativeResourceDir; }

		static const std::fs::path& GetShaderCSOPath() { return m_relativeDir_ShaderCSO; }

		static std::fs::path MakePathStrKey(const std::fs::path& _fullPath);

	private:
		static void Release();

	private:
		static std::filesystem::path m_absoluteResourceDir;
		static std::filesystem::path m_relativeResourceDir;
		
		static std::filesystem::path m_relativeDir_ShaderCSO;
	};


}

