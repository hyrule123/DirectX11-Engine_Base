#include "Engine/Entity.h"

#include "Engine/define_Macro.h"


namespace ehw
{
	UINT32 Entity::g_nextID{};

	Entity::Entity(const std::string_view key)
		: m_ID(++g_nextID)
		, m_strKey(key)
	{
	}
	Entity::Entity(const Entity& _other)
		: m_ID(++g_nextID)
		, m_strKey(_other.m_strKey)
	{
	}
	Entity::~Entity()
	{
	}
}
