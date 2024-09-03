#include "Engine/Manager/InstanceManager.h"

#include "Engine/Util/AtExit.h"

namespace ehw{
	InstanceManager::InstanceManager()
	{
	}
	InstanceManager::~InstanceManager()
	{
	}
	Entity* InstanceManager::Instantiate(const std::string_view key)
	{
		Entity* ret = nullptr;
		auto iter = m_ctors.find(key);
		if (iter != m_ctors.end()) {
			ret = iter->second();
		}

		return ret;
	}
}
