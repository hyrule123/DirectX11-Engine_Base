
#include "Entity.h"

#include <Engine/define_Macro.h>

#include <Engine/Util/AtExit.h>
namespace core
{
	uint32 Entity::s_next_instance_ID = 0;
	uint32 Entity::s_next_class_ID = 0;

	Entity::Entity()
		: m_instance_ID{ ++s_next_instance_ID }
		, m_concrete_class_ID{}
		, m_concrete_class_name{}
	{
	}

	void Entity::init()
	{
		m_concrete_class_ID = get_concrete_class_ID_internal();
		m_concrete_class_name = get_concrete_class_name_internal();
	}

	Entity::~Entity()
	{
	}

	Entity::Entity(const Entity& _other)
		: m_instance_ID{ ++s_next_instance_ID }
		, m_concrete_class_ID{ _other.m_concrete_class_ID }
		, m_concrete_class_name{ _other.m_concrete_class_name }
	{
	}

	EntityFactory::EntityFactory()
		: m_ctors{}
		, m_next_class_ID{}
	{}
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
		}
		return ret;
	}
}
