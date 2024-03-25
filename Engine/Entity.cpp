#include "Engine/Entity.h"

#include "Engine/define_Macro.h"


namespace ehw
{
	UINT32 Entity::gIDNext{};

	Entity::Entity()
		: mID(++gIDNext)
		, m_strKey()
	{
	}
	Entity::Entity(const Entity& _other)
		: mID(++gIDNext)
		, m_strKey(_other.m_strKey)
	{
	}
	Entity::Entity(Entity&& _move) noexcept
		: m_strKey(_move.m_strKey)
		, mID(_move.mID)
	{
	}
	Entity::~Entity()
	{
	}
}
