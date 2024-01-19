#include "Entity.h"

#include "define_Macro.h"


namespace ehw
{
	UINT32 Entity::gIDNext{};

	Entity::Entity()
		: mID(++gIDNext)
		, mStrKey()
	{
	}
	Entity::Entity(const Entity& _other)
		: mID(++gIDNext)
		, mStrKey(_other.mStrKey)
	{
	}
	Entity::Entity(Entity&& _move) noexcept
		: mStrKey(_move.mStrKey)
		, mID(_move.mID)
	{
	}
	Entity::~Entity()
	{
	}
}
