#include "Engine/Resource/Resource.h"

#include "Engine/Manager/ResourceManager.h"

namespace ehw
{
	Resource::Resource(const std::string_view key, const std::type_info& _info)
		: Entity(key)
		, mbEngineDefaultRes()
		, m_ResourceTypeInfo(typeid(Resource))
	{
	}

	Resource::~Resource()
	{
	}

	eResult Resource::Save(const std::fs::path& _baseDir, const std::fs::path& _strKeyPath) const
	{
		ERROR_MESSAGE("저장을 지원하지 않는 리소스 타입입니다.");
		return eResult::Fail_NotImplemented;
	}
}
