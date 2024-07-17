#include "Engine/Resource/Resource.h"

#include "Engine/Manager/ResourceManager.h"

namespace ehw
{
	namespace strKey::Json::Resource
	{
		STRKEY_DECLARE(m_ComCategory);
	}

	


	Resource::Resource(const std::type_info& _info)
		: mbEngineDefaultRes()
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
