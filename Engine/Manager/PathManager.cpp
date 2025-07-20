
#include "PathManager.h"

#include <Engine/Util/AtExit.h>

namespace core
{
	PathManager::PathManager()
		: m_absolute_resource_directory{}
		, m_relative_resource_directory{}
		, m_relative_shader_directory{}
	{}
	void PathManager::init()
	{
		//에러가 발생하지 않게 디렉토리가 없을 경우 생성해주는 작업까지 진행
		m_absolute_resource_directory = std::filesystem::current_path().parent_path().parent_path().parent_path();
		m_absolute_resource_directory /= name::path::directory::resource::Resource;
		if (false == std::fs::exists(m_absolute_resource_directory))
		{
			std::fs::create_directories(m_absolute_resource_directory);
		}

		m_relative_resource_directory = m_absolute_resource_directory.lexically_relative(std::fs::current_path());
		ASSERT(std::fs::exists(m_relative_resource_directory), "절대경로와 상대경로가 일치하지 않습니다.");

		m_relative_shader_directory = ".";
		m_relative_shader_directory /= core::name::path::directory::CompiledShader;
		if (false == std::fs::exists(m_relative_shader_directory))
		{
			std::fs::create_directories(m_relative_shader_directory);
		}

		AtExit::add_func(PathManager::destroy_inst);
	}


	PathManager::~PathManager()
	{
		m_absolute_resource_directory.clear();
		m_relative_resource_directory.clear();
		m_relative_shader_directory.clear();
	}

	std::fs::path PathManager::make_path_strkey(const std::fs::path& _fullPath)
	{
		std::fs::path strKeyPath{};
		std::fs::path newPath = _fullPath.lexically_relative(m_absolute_resource_directory);

		//비어있거나, Res 바깥의 폴더(../)이면 안됨
		if (newPath.empty() || (*newPath.begin()) == "..")
		{
			NOTIFICATION("리소스 파일은 반드시 Res 폴더의 안쪽에 있어야 합니다.");
		}
		else
		{
			for (auto iter = ++newPath.begin(); iter != newPath.end(); ++iter)
			{
				strKeyPath /= *iter;
			}
		}
		return strKeyPath;
	}
}

