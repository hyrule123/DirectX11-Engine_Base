#include "Engine/Entity.h"

#include "Engine/define_Macro.h"


namespace ehw
{
	UINT32 Entity::g_nextID{};

	Entity::Entity()
		: m_ID(++g_nextID)
		, m_strKey()
	{
	}
	Entity::Entity(const Entity& _other)
		: m_ID(++g_nextID)
		, m_strKey(_other.m_strKey)
	{
	}
	Entity::Entity(Entity&& _move) noexcept
		: m_strKey(_move.m_strKey)
		, m_ID(_move.m_ID)
	{
	}
	Entity::~Entity()
	{
	}
	void Entity::Swap(Entity& _other)
	{
		m_strKey.swap(_other.m_strKey);
	}
}
