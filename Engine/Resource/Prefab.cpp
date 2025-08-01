
#include "Prefab.h"

#include <Engine/Util/Serialize/json.h>

#include <Engine/Game/GameObject.h>

#include <Engine/Manager/ResourceManager.h>

namespace core
{
	Prefab::Prefab()
		: Resource()
		, m_prefab()
	{
	}

	Prefab::~Prefab()
	{
	}
	eResult Prefab::save(const std::fs::path& _base_directory) const
	{
		if (m_prefab)
		{
			return m_prefab->save_file_json(_base_directory / get_res_filename());
		}

		return eResult::Fail_Nullptr;
	}
	eResult Prefab::load(const std::fs::path& _base_directory)
	{
		m_prefab = std::make_shared<GameObject>();

		eResult result = m_prefab->load_file_json(_base_directory / get_res_filename());
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
			obj = std::static_pointer_cast<GameObject>(m_prefab->clone());
		}

		return obj;
	}
}

