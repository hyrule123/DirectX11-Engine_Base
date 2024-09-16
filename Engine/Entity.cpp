#include "Engine/Entity.h"

#include "Engine/define_Macro.h"

#include "Engine/Util/AtExit.h"
namespace ehw
{
	UINT32 Entity::g_nextID{};

	Entity::Entity(const std::string_view key)
		: m_ID(++g_nextID)
		, m_concrete_class_name(key)
	{
	}
	Entity::Entity(const Entity& _other)
		: m_ID(++g_nextID)
		, m_concrete_class_name(_other.m_concrete_class_name)
	{
	}
	Entity::~Entity()
	{
	}

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
