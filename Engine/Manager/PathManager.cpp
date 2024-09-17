#include "PathManager.h"
#include "Engine/Util/AtExit.h"

namespace ehw
{
	PathManager::PathManager()
		: m_absoluteResourceDir{}
		, m_relativeResourceDir{}
		, m_relativeDir_ShaderCSO{}
	{
	}
	PathManager::~PathManager()
	{
	}
	void PathManager::init()
	{
		AtExit::AddFunc(std::bind(&PathManager::release, this));

		//에러가 발생하지 않게 디렉토리가 없을 경우 생성해주는 작업까지 진행
		m_absoluteResourceDir = std::filesystem::current_path().parent_path().parent_path().parent_path();
		m_absoluteResourceDir /= strKey::path::directory::resource::Resource;
		if (false == std::fs::exists(m_absoluteResourceDir))
		{
			std::fs::create_directories(m_absoluteResourceDir);
		}

		m_relativeResourceDir = m_absoluteResourceDir.lexically_relative(std::fs::current_path());
		ASSERT(std::fs::exists(m_relativeResourceDir), "절대경로와 상대경로가 일치하지 않습니다.");

		m_relativeDir_ShaderCSO = ".";
		m_relativeDir_ShaderCSO /= ehw::strKey::path::directory::CompiledShader;
		if (false == std::fs::exists(m_relativeDir_ShaderCSO))
		{
			std::fs::create_directories(m_relativeDir_ShaderCSO);
		}
	}


	void PathManager::release()
	{
		m_absoluteResourceDir.clear();
		m_relativeResourceDir.clear();

		m_relativeDir_ShaderCSO.clear();
	}

	std::fs::path PathManager::MakePathStrKey(const std::fs::path& _fullPath)
	{
		std::fs::path strKeyPath{};
		std::fs::path newPath = _fullPath.lexically_relative(m_absoluteResourceDir);

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

