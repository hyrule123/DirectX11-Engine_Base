
#include "Resource.h"

#include <Engine/Manager/ResourceManager.h>

namespace core
{
	Resource::Resource()
		: Entity()
		, m_res_pathkey()
		, m_res_type()
	{
	}

	void Resource::init()
	{
		Super::init();
		m_res_type = get_resource_type_internal();
	}

	Resource::~Resource()
	{
	}
}
