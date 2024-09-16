#include "Engine/Resource/Prefab.h"

#include "Engine/Util/Serialize/json.h"

#include "Engine/Game/GameObject.h"

#include "Engine/Manager/ResourceManager.h"

namespace ehw
{
	Prefab::Prefab()
		: Resource(Prefab::concrete_class_name)
		, m_prefab()
	{
	}

	Prefab::~Prefab()
	{
	}
	eResult Prefab::save(const std::fs::path& _baseDir, const std::fs::path& _key_path) const
	{
		if (m_prefab)
		{
			return m_prefab->SaveFile_Json(_baseDir / _key_path);
		}

		return eResult::Fail_Nullptr;
	}
	eResult Prefab::load(const std::fs::path& _baseDir, const std::fs::path& _key_path)
	{
		m_prefab = std::make_shared<GameObject>();

		eResult result = m_prefab->LoadFile_Json(_baseDir / _key_path);
		if (eResult_fail(result))
		{
			m_prefab = nullptr;
		}
		return result;
	}


	std::shared_ptr<GameObject> Prefab::Instantiate()
	{
		std::shared_ptr<GameObject> obj{};
		
		if (m_prefab)
		{
			obj = std::shared_ptr<GameObject>(m_prefab->Clone());
		}

		return obj;
	}
}

