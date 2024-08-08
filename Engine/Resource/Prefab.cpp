#include "Engine/Resource/Prefab.h"

#include "Engine/Util/Serialize/json.h"

#include "Engine/Scene/GameObject.h"

#include "Engine/Manager/ResourceManager.h"

namespace ehw
{
	namespace strKey::Json::Prefab
	{
		STRKEY_DECLARE(m_prefab);
	}

	

	Prefab::Prefab()
		: Resource(typeid(Prefab))
		, m_prefab()
	{
	}

	Prefab::~Prefab()
	{
	}
	eResult Prefab::Save(const std::fs::path& _baseDir, const std::fs::path& _strKeyPath) const
	{
		if (m_prefab)
		{
			return m_prefab->SaveFile_Json(_baseDir / _strKeyPath);
		}

		return eResult::Fail_Nullptr;
	}
	eResult Prefab::Load(const std::fs::path& _baseDir, const std::fs::path& _strKeyPath)
	{
		m_prefab = std::make_shared<GameObject>();

		eResult result = m_prefab->LoadFile_Json(_baseDir / _strKeyPath);
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

