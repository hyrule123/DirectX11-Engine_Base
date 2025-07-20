
#include "Entity.h"

#include <Engine/define_Macro.h>

#include <Engine/Util/AtExit.h>
namespace core
{
	UINT32 Entity::g_next_ID{};

	Entity::Entity(const std::string_view _class_concrete_name)
		: m_ID(++g_next_ID)
		, m_static_type_name(_class_concrete_name)
	{
	}

	void Entity::init()
	{
	}

	Entity::~Entity()
	{
	}

	Entity::Entity(const Entity& _other)
		: m_ID(++g_next_ID)
		, m_static_type_name(_other.m_static_type_name)
	{
	}

	EntityFactory::EntityFactory() {}
	void EntityFactory::init()
	{
		AtExit::add_func(EntityFactory::destroy_inst);
	}

	EntityFactory::~EntityFactory()	
	{
	}
	s_ptr<Entity> EntityFactory::instantiate(const std::string_view key)
	{
		s_ptr<Entity> ret = nullptr;
		auto iter = m_ctors.find(key);
		if (iter != m_ctors.end()) {
			ret = iter->second();
			ret->init();
		}
		return ret;
	}
}
