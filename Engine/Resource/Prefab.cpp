
#include "Prefab.h"

#include <Engine/Util/Serialize/json.h>

#include <Engine/Game/GameObject.h>

#include <Engine/Manager/ResourceManager.h>

namespace core
{
	Prefab::Prefab()
		: Resource(Prefab::s_static_type_name)
		, m_prefab()
	{
	}

	Prefab::~Prefab()
	{
	}
	eResult Prefab::save(const std::fs::path& _base_directory, const std::fs::path& _resource_name) const
	{
		if (m_prefab)
		{
			return m_prefab->SaveFile_Json(_base_directory / _resource_name);
		}

		return eResult::Fail_Nullptr;
	}
	eResult Prefab::load(const std::fs::path& _base_directory, const std::fs::path& _resource_name)
	{
		m_prefab = std::make_shared<GameObject>();

		eResult result = m_prefab->LoadFile_Json(_base_directory / _resource_name);
		if (eResult_fail(result))
		{
			m_prefab = nullptr;
		}
		return result;
	}


	s_ptr<GameObject> Prefab::instantiate()
	{
		s_ptr<GameObject> obj{};
		
		if (m_prefab)
		{
			obj = std::static_pointer_cast<GameObject>(m_prefab->Clone());
		}

		return obj;
	}
}

