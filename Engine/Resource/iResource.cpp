#include "iResource.h"


#include "../Manager/ResourceManager.h"

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
}
