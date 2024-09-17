#include "Engine/Resource/Resource.h"

#include "Engine/Manager/ResourceManager.h"

namespace ehw
{
	Resource::Resource(const std::string_view key)
		: Entity(key)
		, mbEngineDefaultRes()
	{
	}

	Resource::~Resource()
	{
	}

	eResult Resource::save(const std::fs::path& _base_directory, const std::fs::path& _key_path) const
	{
		ERROR_MESSAGE("저장을 지원하지 않는 리소스 타입입니다.");
		return eResult::Fail_NotImplemented;
	}
}
