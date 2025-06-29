#include "Engine/Game/Entity.h"

#include "Engine/define_Macro.h"

#include "Engine/Util/AtExit.h"
namespace core
{
	UINT32 Entity::g_nextID{};

	Entity::Entity(const std::string_view _class_concrete_name)
		: m_ID(++g_nextID)
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
		: m_ID(++g_nextID)
		, m_static_type_name(_other.m_static_type_name)
	{
	}

	EntityFactory::EntityFactory()
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
