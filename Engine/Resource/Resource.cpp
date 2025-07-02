
#include "Resource.h"

#include <Engine/Manager/ResourceManager.h>

namespace core
{
	Resource::Resource(const std::string_view key)
		: Entity(key)
		, mbEngineDefaultRes()
	{
	}

	Resource::~Resource()
	{
	}

	eResult Resource::save(const std::fs::path& _base_directory, const std::fs::path& _resource_name) const
	{
		ERROR_MESSAGE("파일로 저장을 지원하지 않는 리소스 타입입니다.");
		return eResult::Fail_NotImplemented;
	}
	eResult Resource::load(const std::fs::path& _base_directory, const std::fs::path& _resource_name)
	{
		ERROR_MESSAGE("파일로부터 불러오기를 지원하지 않는 리소스 타입입니다.");
		return eResult::Fail_NotImplemented;
	};
}
