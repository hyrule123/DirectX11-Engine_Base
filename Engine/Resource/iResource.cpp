#include "iResource.h"


#include "Manager/ResourceManager.h"

namespace ehw
{
	namespace strKey::Json::iResource
	{
		STRKEY_DECLARE(m_ComCategory);
	}

	


	iResource::iResource(const std::type_info& _info)
		: mbEngineDefaultRes()
		, m_ResourceTypeInfo(typeid(iResource))
	{
	}

	iResource::~iResource()
	{
	}

	eResult iResource::Save(const std::fs::path& _baseDir, const std::fs::path& _strKeyPath) const
	{
		ERROR_MESSAGE("저장을 지원하지 않는 리소스 타입입니다.");
		return eResult::Fail_NotImplemented;
	}
}
