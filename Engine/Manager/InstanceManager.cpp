#include "Engine/Manager/InstanceManager.h"

#include "Engine/Util/AtExit.h"

namespace ehw{
	InstanceManager::InstanceManager()
	{
	}
	InstanceManager::~InstanceManager()
	{
	}
	void InstanceManager::init()
	{
		AtExit::AddFunc(std::bind(&InstanceManager::release, this));
	}
	void InstanceManager::release()
	{
		m_ctors.clear();
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
